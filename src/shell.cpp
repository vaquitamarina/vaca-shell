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
