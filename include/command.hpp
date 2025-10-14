/**
 * @file command.hpp
 * @brief Estructuras de datos para representar comandos parseados
 * 
 * Define las estructuras Command (comando individual) y Pipeline
 * (secuencia de comandos conectados por pipes)
 */

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <vector>

/**
 * @struct Command
 * @brief Representa un comando individual con sus argumentos y redirecciones
 */
struct Command {
    std::string program;                 ///< Nombre del programa a ejecutar
    std::vector<std::string> args;       ///< Argumentos del comando
    std::string input_file;              ///< Archivo de entrada (< file)
    std::string output_file;             ///< Archivo de salida (> o >> file)
    bool append_output;                  ///< true si es >>, false si es >
    bool background;                     ///< true si termina con &
    
    /// Constructor por defecto
    Command();
    
    /**
     * @brief Convierte los argumentos a formato argv[] para execvp()
     * @return Array de char* terminado en NULL
     */
    char** to_argv() const;
    
    /**
     * @brief Libera memoria del argv generado
     * @param argv Array a liberar
     */
    static void free_argv(char** argv);
    
    /**
     * @brief Verifica si el comando está vacío
     * @return true si no hay programa
     */
    bool is_empty() const;
    
    /**
     * @brief Imprime el comando (para debugging)
     */
    void print() const;
};

/**
 * @struct Pipeline
 * @brief Representa una secuencia de comandos conectados por pipes
 * 
 * Ejemplo: cmd1 | cmd2 | cmd3 &
 */
struct Pipeline {
    std::vector<Command> commands;       ///< Lista de comandos
    bool background;                     ///< Si todo el pipeline va en background
    
    /// Constructor
    Pipeline();
    
    /**
     * @brief Verifica si el pipeline está vacío
     */
    bool is_empty() const;
    
    /**
     * @brief Verifica si es un comando simple (sin pipes)
     */
    bool is_simple() const;
    
    /**
     * @brief Retorna el número de comandos
     */
    size_t size() const;
};

#endif // COMMAND_HPP
