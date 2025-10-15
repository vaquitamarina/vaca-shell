/**
 * @file builtins.hpp
 * @brief Comandos internos (built-ins)
 * 
 * Comandos que se ejecutan directamente en el proceso de la shell
 * sin hacer fork(): cd, pwd, help, history, alias, jobs, exit
 */

#ifndef BUILTINS_HPP
#define BUILTINS_HPP

#include "command.hpp"
#include <string>
#include <vector>
#include <map>

/**
 * @class Builtins
 * @brief Gestor de comandos internos
 */
class Builtins {
private:
    std::vector<std::string> history;           ///< Historial de comandos
    std::map<std::string, std::string> aliases; ///< Aliases definidos
    int history_index;
    
public:
    Builtins();
    /**
     * @brief Verifica si un comando es interno
     * @param cmd Nombre del comando
     * @return true si es built-in
     */
    bool is_builtin(const std::string& cmd) const;
    
    /**
     * @brief Ejecuta un comando interno
     * @param cmd Comando a ejecutar
     * @return Código de salida
     */
    int execute_builtin(const Command& cmd);
    
    /**
     * @brief cd - Cambiar directorio (usa chdir())
     * @param args Argumentos del comando
     * @return 0 si éxito, 1 si error
     */
    int builtin_cd(const std::vector<std::string>& args);
    
    /**
     * @brief pwd - Mostrar directorio actual (usa getcwd())
     * @param args Argumentos del comando
     * @return 0 si éxito, 1 si error
     */
    int builtin_pwd(const std::vector<std::string>& args);
    
    /**
     * @brief exit/salir - Salir de la shell
     * @param args Código de salida opcional
     * @return No retorna (llama a exit())
     */
    int builtin_exit(const std::vector<std::string>& args);
    
    /**
     * @brief help - Mostrar ayuda
     * @param args Argumentos (no usados)
     * @return 0
     */
    int builtin_help(const std::vector<std::string>& args);
    
    /**
     * @brief history - Mostrar historial de comandos
     * @param args Argumentos (no usados)
     * @return 0
     */
    int builtin_history(const std::vector<std::string>& args);
    
    /**
     * @brief alias - Crear o mostrar aliases
     * @param args nombre=valor o vacío para listar
     * @return 0
     */
    int builtin_alias(const std::vector<std::string>& args);
    
    /**
     * @brief jobs - Mostrar trabajos en background
     * @param args Argumentos (no usados)
     * @return 0
     */
    int builtin_jobs(const std::vector<std::string>& args);
    
    /**
     * @brief Agrega un comando al historial
     * @param line Línea de comando
     */
    void add_to_history(const std::string& line);
    
    /**
     * @brief Expande un alias si existe
     * @param cmd Comando a expandir
     * @return Comando expandido o el original
     */
    std::string expand_alias(const std::string& cmd) const;

    std::string get_previous_command();

    std::string get_next_command();
    
    void reset_history_index();
};

#endif // BUILTINS_HPP
