/**
 * @file signals.hpp
 * @brief Manejo de señales POSIX
 * 
 * Configuración de manejadores usando sigaction() para SIGINT,
 * SIGCHLD y otras señales relevantes
 */

#ifndef SIGNALS_HPP
#define SIGNALS_HPP

#include <signal.h>

/**
 * @class SignalHandler
 * @brief Gestor de señales del sistema
 */
class SignalHandler {
public:
    /**
     * @brief Inicializa manejadores de señales con sigaction()
     * 
     * Configura SIGINT (Ctrl+C) y SIGCHLD (hijo terminado)
     */
    static void setup_signals();
    
    /**
     * @brief Manejador para SIGINT (Ctrl+C)
     * 
     * No debe terminar la shell, solo el proceso hijo actual
     * 
     * @param signo Número de señal
     */
    static void sigint_handler(int signo);
    
    /**
     * @brief Manejador para SIGCHLD (proceso hijo terminado)
     * 
     * Recolecta procesos zombie en segundo plano
     * 
     * @param signo Número de señal
     */
    static void sigchld_handler(int signo);
    
    /**
     * @brief Restaura manejadores por defecto
     */
    static void restore_default_handlers();
    
    /**
     * @brief Ignora SIGINT temporalmente
     */
    static void ignore_sigint();
    
    /**
     * @brief Restaura el manejador de SIGINT
     */
    static void restore_sigint();
};

#endif // SIGNALS_HPP
