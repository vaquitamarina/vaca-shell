#include "shell.hpp"
#include "signals.hpp"
#include <iostream>
#include <unistd.h>
#include <limits.h>
#include <cstring>

using namespace std;

VacaShell::VacaShell() : running(false), last_exit_code(0) {}

VacaShell::~VacaShell() {}

void VacaShell::initialize() {
    SignalHandler::setup_signals();
    
    cout << "\n";
    cout << "╔═══════════════════════════════════════════════════════════╗\n";
    cout << "║                                                           ║\n";
    cout << "║               🐮  VacaShell v1.0  🐮                      ║\n";
    cout << "║                                                           ║\n";
    cout << "║          Mini-shell POSIX en C++ para Linux               ║\n";
    cout << "║                                                           ║\n";
    cout << "║   Características implementadas:                          ║\n";
    cout << "║   ✓ Ejecución de comandos (fork + execvp)                ║\n";
    cout << "║   ✓ Resolución de rutas (/bin/ por defecto)              ║\n";
    cout << "║   ✓ Redirecciones: >, >>, <                              ║\n";
    cout << "║   ✓ Pipes: cmd1 | cmd2 | cmd3                            ║\n";
    cout << "║   ✓ Ejecución en segundo plano: &                        ║\n";
    cout << "║   ✓ Comandos internos: cd, pwd, help, etc.               ║\n";
    cout << "║   ✓ Manejo de señales: SIGINT, SIGCHLD                   ║\n";
    cout << "║                                                           ║\n";
    cout << "║   Escribe 'help' para ver ayuda completa                  ║\n";
    cout << "║   Escribe 'salir' o 'exit' para salir                     ║\n";
    cout << "║                                                           ║\n";
    cout << "╚═══════════════════════════════════════════════════════════╝\n";
    cout << "\n";
}

void VacaShell::show_prompt() const {
    const char* user = getenv("USER");
    if (user == nullptr) user = "usuario";
    
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) != 0) {
        strcpy(hostname, "localhost");
    }
    
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == nullptr) {
        strcpy(cwd, "?");
    }
    
    string dir_name = cwd;
    size_t last_slash = dir_name.find_last_of('/');
    if (last_slash != string::npos && last_slash < dir_name.length() - 1) {
        dir_name = dir_name.substr(last_slash + 1);
    }
    else if (dir_name == "/") {
        dir_name = "/";
    }
    
    string symbol = (last_exit_code == 0) ? "🐮" : "❌";
    
    cout << "\033[1;32m" << user << "@" << hostname << "\033[0m"
            << ":\033[1;34m" << dir_name << "\033[0m"
            << " " << symbol << " $ ";
    cout.flush();
}

bool VacaShell::read_line(string& line) {
    if (!getline(cin, line)) {
        if (cin.eof()) {
            cout << "\n¡Hasta luego! 👋\n";
            return false;
        }
        cin.clear();
        return true;
    }
    return true;
}

void VacaShell::process_line(const string& line) {
    if (!Parser::is_empty_or_comment(line)) {
        builtins.add_to_history(line);
    }
    
    Pipeline pipeline = Parser::parse_line(line);
    
    if (pipeline.is_empty()) {
        return;
    }
    
    const Command& first_cmd = pipeline.commands[0];
    
    string expanded = builtins.expand_alias(first_cmd.program);
    if (expanded != first_cmd.program) {
        string new_line = expanded;
        for (const auto& arg : first_cmd.args) {
            new_line += " " + arg;
        }
        pipeline = Parser::parse_line(new_line);
        
        if (pipeline.is_empty()) {
            return;
        }
    }
    
    if (pipeline.is_simple() && builtins.is_builtin(pipeline.commands[0].program)) {
        last_exit_code = builtins.execute_builtin(pipeline.commands[0]);
        
        if (pipeline.commands[0].program == "jobs" || 
            pipeline.commands[0].program == "trabajos") {
            executor.show_jobs();
        }
    }
    else {
        last_exit_code = executor.execute_pipeline(pipeline);
    }
}

void VacaShell::run() {
    running = true;
    string line;
    
    while (running) {
        show_prompt();
        
        if (!read_line(line)) {
            break;
        }
        
        if (!line.empty()) {
            process_line(line);
        }
    }
}

void VacaShell::stop() {
    running = false;
}

int VacaShell::get_last_exit_code() const {
    return last_exit_code;
}
