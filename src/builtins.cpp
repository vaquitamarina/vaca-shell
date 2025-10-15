#include "builtins.hpp"
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <limits.h>
#include <errno.h>
#include <cstring>

bool Builtins::is_builtin(const std::string& cmd) const {
    return cmd == "cd" || cmd == "pwd" || cmd == "exit" || cmd == "salir" ||
        cmd == "help" || cmd == "ayuda" || cmd == "history" || cmd == "historial" ||
        cmd == "alias" || cmd == "jobs" || cmd == "trabajos";
}

int Builtins::execute_builtin(const Command& cmd) {
    const std::string& name = cmd.program;
    
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

int Builtins::builtin_cd(const std::vector<std::string>& args) {
    std::string path;
    
    if (args.empty()) {
        const char* home = getenv("HOME");
        if (home == nullptr) {
            std::cerr << "cd: Error - Variable HOME no definida" << std::endl;
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

int Builtins::builtin_pwd(const std::vector<std::string>& args) {
    (void)args; // No usado
    
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        std::cout << cwd << std::endl;
        return 0;
    }
    else {
        perror("pwd");
        return 1;
    }
}

int Builtins::builtin_exit(const std::vector<std::string>& args) {
    int exit_code = 0;
    
    if (!args.empty()) {
        try {
            exit_code = std::stoi(args[0]);
        }
        catch (...) {
            std::cerr << "exit: Argumento inválido: " << args[0] << std::endl;
            return 1;
        }
    }
    
    std::cout << "\n¡Hasta luego! 👋\n" << std::endl;
    exit(exit_code);
}

int Builtins::builtin_help(const std::vector<std::string>& args) {
    (void)args;
    
    std::cout << "\n";
    std::cout << "╔═══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║              VacaShell - Mini-shell POSIX en C++              ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════════════╝\n\n";
    
    std::cout << "COMANDOS INTERNOS:\n";
    std::cout << "  cd [dir]         - Cambiar de directorio\n";
    std::cout << "  pwd              - Mostrar directorio actual\n";
    std::cout << "  help, ayuda      - Mostrar esta ayuda\n";
    std::cout << "  history          - Mostrar historial de comandos\n";
    std::cout << "  alias [x=y]      - Crear o mostrar aliases\n";
    std::cout << "  jobs, trabajos   - Mostrar trabajos en segundo plano\n";
    std::cout << "  exit, salir [n]  - Salir (código n opcional)\n\n";
    
    std::cout << "CARACTERÍSTICAS:\n";
    std::cout << "  comando > file   - Redirección de salida (sobrescribir)\n";
    std::cout << "  comando >> file  - Redirección de salida (agregar)\n";
    std::cout << "  comando < file   - Redirección de entrada\n";
    std::cout << "  cmd1 | cmd2      - Pipes (tuberías)\n";
    std::cout << "  comando &        - Ejecución en segundo plano\n";
    std::cout << "  # comentario     - Líneas de comentario\n\n";
    
    std::cout << "EJEMPLOS:\n";
    std::cout << "  ls -la\n";
    std::cout << "  echo 'Hola mundo' > salida.txt\n";
    std::cout << "  cat archivo.txt | grep patron\n";
    std::cout << "  sleep 10 &\n";
    std::cout << "  cat < entrada.txt > salida.txt\n";
    std::cout << "  ps aux | grep bash | wc -l\n\n";
    
    std::cout << "RUTAS:\n";
    std::cout << "  - Rutas absolutas se ejecutan tal cual: /usr/bin/ls\n";
    std::cout << "  - Rutas relativas se ejecutan tal cual: ./script.sh\n";
    std::cout << "  - Nombres simples se buscan en /bin/ y /usr/bin/\n\n";
    
    return 0;
}