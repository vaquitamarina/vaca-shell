#include "shell.hpp"
#include "signals.hpp"
#include <iostream>
#include <unistd.h>
#include <limits.h>
#include <cstring>

VacaShell::VacaShell() : running(false), last_exit_code(0) {}

VacaShell::~VacaShell() {}

void VacaShell::initialize() {
    SignalHandler::setup_signals();
    
    std::cout << "\n";
    std::cout << "╔═══════════════════════════════════════════════════════════╗\n";
    std::cout << "║                                                           ║\n";
    std::cout << "║               🐮  VacaShell v1.0  🐮                      ║\n";
    std::cout << "║                                                           ║\n";
    std::cout << "║          Mini-shell POSIX en C++ para Linux               ║\n";
    std::cout << "║                                                           ║\n";
    std::cout << "║   Características implementadas:                          ║\n";
    std::cout << "║   ✓ Ejecución de comandos (fork + execvp)                ║\n";
    std::cout << "║   ✓ Resolución de rutas (/bin/ por defecto)              ║\n";
    std::cout << "║   ✓ Redirecciones: >, >>, <                              ║\n";
    std::cout << "║   ✓ Pipes: cmd1 | cmd2 | cmd3                            ║\n";
    std::cout << "║   ✓ Ejecución en segundo plano: &                        ║\n";
    std::cout << "║   ✓ Comandos internos: cd, pwd, help, etc.               ║\n";
    std::cout << "║   ✓ Manejo de señales: SIGINT, SIGCHLD                   ║\n";
    std::cout << "║                                                           ║\n";
    std::cout << "║   Escribe 'help' para ver ayuda completa                 ║\n";
    std::cout << "║   Escribe 'salir' o 'exit' para salir                    ║\n";
    std::cout << "║                                                           ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════════╝\n";
    std::cout << "\n";
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
    
    std::string dir_name = cwd;
    size_t last_slash = dir_name.find_last_of('/');
    if (last_slash != std::string::npos && last_slash < dir_name.length() - 1) {
        dir_name = dir_name.substr(last_slash + 1);
    }
    else if (dir_name == "/") {
        dir_name = "/";
    }
    
    std::string symbol = (last_exit_code == 0) ? "🐮" : "❌";
    
    std::cout << "\033[1;32m" << user << "@" << hostname << "\033[0m"
              << ":\033[1;34m" << dir_name << "\033[0m"
              << " " << symbol << " $ ";
    std::cout.flush();
}

bool VacaShell::read_line(std::string& line) {
    if (!std::getline(std::cin, line)) {
        if (std::cin.eof()) {
            std::cout << "\n¡Hasta luego! 👋\n";
            return false;
        }
        std::cin.clear();
        return true;
    }
    return true;
}
