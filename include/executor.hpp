/**
 * @file executor.hpp
 * @brief Ejecutor de comandos usando llamadas POSIX
 * 
 * Implementa la ejecución de comandos mediante fork(), execvp(),
 * wait(), waitpid(), pipe(), dup2(), open(), close()
 */

#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include "command.hpp"
#include <map>
#include <string>
#include <sys/types.h>

/**
 * @class Executor
 * @brief Gestor de ejecución de comandos y procesos
 */
class Executor {
private:
    std::map<pid_t, std::string> background_jobs;  ///< Trabajos en segundo plano
    
public:
    /**
     * @brief Ejecuta un pipeline completo
     * @param pipeline Pipeline a ejecutar
     * @return Código de salida del último comando
     */
    int execute_pipeline(const Pipeline& pipeline);
    
    /**
     * @brief Ejecuta un comando simple (sin pipes)
     * @param cmd Comando a ejecutar
     * @return Código de salida
     */
    int execute_simple_command(const Command& cmd);
    
    /**
     * @brief Ejecuta pipeline con múltiples comandos (pipes)
     * @param pipeline Pipeline con comandos conectados
     * @return Código de salida del último comando
     */
    int execute_piped_commands(const Pipeline& pipeline);
    
    /**
     * @brief Resuelve la ruta completa de un comando
     * 
     * Si el comando contiene '/', se usa tal cual (absoluta/relativa).
     * Si no, busca en /bin/ y /usr/bin/
     * 
     * @param program Nombre del programa
     * @return Ruta completa del programa
     */
    std::string resolve_path(const std::string& program);
    
    /**
     * @brief Configura redirecciones de entrada/salida
     * 
     * Usa open(), dup2() y close() para redirigir stdin/stdout
     * 
     * @param cmd Comando con información de redirecciones
     * @return true si se configuró correctamente
     */
    bool setup_redirections(const Command& cmd);
    
    /**
     * @brief Recolecta procesos hijos terminados (waitpid con WNOHANG)
     * 
     * Usa waitpid() no bloqueante para recolectar procesos zombie
     */
    void collect_background_jobs();
    
    /**
     * @brief Muestra trabajos en segundo plano activos
     */
    void show_jobs() const;
    
    /**
     * @brief Agrega un trabajo en segundo plano
     * @param pid PID del proceso
     * @param command Descripción del comando
     */
    void add_background_job(pid_t pid, const std::string& command);
};

#endif // EXECUTOR_HPP
