#include "builtins.hpp"
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <limits.h>
#include <errno.h>
#include <cstring>

using namespace std;

bool Builtins::is_builtin(const string& cmd) const {
    return cmd == "cd" || cmd == "pwd" || cmd == "exit" || cmd == "salir" ||
        cmd == "help" || cmd == "ayuda" || cmd == "history" || cmd == "historial" ||
        cmd == "alias" || cmd == "jobs" || cmd == "trabajos";
}

int Builtins::execute_builtin(const Command& cmd) {
    const string& name = cmd.program;
    
    if (name == "cd") {
        return builtin_cd(cmd.args);
    }
    else if (name == "pwd") {
        return builtin_pwd(cmd.args);
    }
    else if (name == "exit" || name == "salir") {
        return builtin_exit(cmd.args);
    }
    else if (name == "help" || name == "ayuda") {
        return builtin_help(cmd.args);
    }
    else if (name == "history" || name == "historial") {
        return builtin_history(cmd.args);
    }
    else if (name == "alias") {
        return builtin_alias(cmd.args);
    }
    else if (name == "jobs" || name == "trabajos") {
        return builtin_jobs(cmd.args);
    }
    
    return 0;
}

int Builtins::builtin_cd(const vector<string>& args) {
    string path;
    
    if (args.empty()) {
        const char* home = getenv("HOME");
        if (home == nullptr) {
            cerr << "cd: Error - Variable HOME no definida" << endl;
            return 1;
        }
        path = home;
    }
    else {
        path = args[0];
    }
    
    if (chdir(path.c_str()) != 0) {
        perror(("cd: " + path).c_str());
        return 1;
    }
    
    return 0;
}

int Builtins::builtin_pwd(const vector<string>& args) {
    (void)args; // No usado
    
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        cout << cwd << endl;
        return 0;
    }
    else {
        perror("pwd");
        return 1;
    }
}

int Builtins::builtin_exit(const vector<string>& args) {
    int exit_code = 0;
    
    if (!args.empty()) {
        try {
            exit_code = stoi(args[0]);
        }
        catch (...) {
            cerr << "exit: Argumento inválido: " << args[0] << endl;
            return 1;
        }
    }
    
    cout << "\n¡Hasta luego! 👋\n" << endl;
    exit(exit_code);
}

int Builtins::builtin_help(const vector<string>& args) {
    (void)args;
    
    cout << "\n";
    cout << "╔═══════════════════════════════════════════════════════════════╗\n";
    cout << "║              VacaShell - Mini-shell POSIX en C++              ║\n";
    cout << "╚═══════════════════════════════════════════════════════════════╝\n\n";
    
    cout << "COMANDOS INTERNOS:\n";
    cout << "  cd [dir]         - Cambiar de directorio\n";
    cout << "  pwd              - Mostrar directorio actual\n";
    cout << "  help, ayuda      - Mostrar esta ayuda\n";
    cout << "  history          - Mostrar historial de comandos\n";
    cout << "  alias [x=y]      - Crear o mostrar aliases\n";
    cout << "  jobs, trabajos   - Mostrar trabajos en segundo plano\n";
    cout << "  exit, salir [n]  - Salir (código n opcional)\n\n";
    
    cout << "CARACTERÍSTICAS:\n";
    cout << "  comando > file   - Redirección de salida (sobrescribir)\n";
    cout << "  comando >> file  - Redirección de salida (agregar)\n";
    cout << "  comando < file   - Redirección de entrada\n";
    cout << "  cmd1 | cmd2      - Pipes (tuberías)\n";
    cout << "  comando &        - Ejecución en segundo plano\n";
    cout << "  # comentario     - Líneas de comentario\n\n";
    
    cout << "EJEMPLOS:\n";
    cout << "  ls -la\n";
    cout << "  echo 'Hola mundo' > salida.txt\n";
    cout << "  cat archivo.txt | grep patron\n";
    cout << "  sleep 10 &\n";
    cout << "  cat < entrada.txt > salida.txt\n";
    cout << "  ps aux | grep bash | wc -l\n\n";
    
    cout << "RUTAS:\n";
    cout << "  - Rutas absolutas se ejecutan tal cual: /usr/bin/ls\n";
    cout << "  - Rutas relativas se ejecutan tal cual: ./script.sh\n";
    cout << "  - Nombres simples se buscan en /bin/ y /usr/bin/\n\n";
    
    return 0;
}

int Builtins::builtin_history(const vector<string>& args) {
    (void)args;
    
    if (history.empty()) {
        cout << "Historial vacío." << endl;
        return 0;
    }
    
    cout << "Historial de comandos:\n";
    for (size_t i = 0; i < history.size(); i++) {
        cout << "  " << (i + 1) << "  " << history[i] << endl;
    }
    
    return 0;
}

int Builtins::builtin_alias(const vector<string>& args) {
    if (args.empty()) {
        if (aliases.empty()) {
            cout << "No hay aliases definidos." << endl;
        }
        else {
            cout << "Aliases definidos:\n";
            for (const auto& pair : aliases) {
                cout << "  " << pair.first << "='" << pair.second << "'\n";
            }
        }
        return 0;
    }
    
    string arg = args[0];
    size_t eq_pos = arg.find('=');
    
    if (eq_pos == string::npos) {
        auto it = aliases.find(arg);
        if (it != aliases.end()) {
            cout << arg << "='" << it->second << "'\n";
        }
        else {
            cout << "Alias '" << arg << "' no definido.\n";
        }
    }
    else {
        string name = arg.substr(0, eq_pos);
        string value = arg.substr(eq_pos + 1);
        
        if (!value.empty() && value.front() == '\'' && value.back() == '\'') {
            value = value.substr(1, value.length() - 2);
        }
        
        aliases[name] = value;
        cout << "Alias '" << name << "' creado.\n";
    }
    
    return 0;
}

int Builtins::builtin_jobs(const vector<string>& args) {
    (void)args;
    // Este comando se complementa con Executor::show_jobs()
    cout << "(El listado de jobs lo muestra el executor)\n";
    return 0;
}

void Builtins::add_to_history(const string& line) {
    if (!line.empty()) {
        history.push_back(line);
        
        // Limitar tamaño del historial
        const size_t MAX_HISTORY = 1000;
        if (history.size() > MAX_HISTORY) {
            history.erase(history.begin());
        }
    }
}

string Builtins::expand_alias(const string& cmd) const {
    auto it = aliases.find(cmd);
    if (it != aliases.end()) {
        return it->second;
    }
    return cmd;
}