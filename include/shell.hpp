/**
 * @file shell.hpp
 * @brief Clase principal de la mini-shell
 * 
 * Coordina todos los componentes: parser, executor, builtins, signals
 * Implementa el loop principal y el prompt personalizado
 */

#ifndef SHELL_HPP
#define SHELL_HPP

#include "parser.hpp"
#include "executor.hpp"
#include "builtins.hpp"
#include <string>

/**
 * @class VacaShell
 * @brief Shell principal que coordina todos los módulos
 */
class VacaShell {
private:
    Executor executor;       ///< Gestor de ejecución
    Builtins builtins;       ///< Comandos internos
    bool running;            ///< Estado de la shell
    int last_exit_code;      ///< Código de salida del último comando
    
    /**
     * @brief Muestra el prompt personalizado
     * 
     * Formato: usuario@hostname:directorio símbolo $
     * El símbolo cambia según el código de salida anterior
     */
    void show_prompt() const;
    
    /**
     * @brief Lee una línea de entrada del usuario
     * @param line String donde guardar la línea
     * @return true si se leyó correctamente, false si EOF
     */
    bool read_line(std::string& line);
    
    /**
     * @brief Procesa y ejecuta una línea de comando
     * @param line Línea a procesar
     */
    void process_line(const std::string& line);
    
public:
    /**
     * @brief Constructor
     */
    VacaShell();
    
    /**
     * @brief Destructor
     */
    ~VacaShell();
    
    /**
     * @brief Inicializa la shell (señales, mensaje de bienvenida)
     */
    void initialize();
    
    /**
     * @brief Loop principal de la shell
     * 
     * Muestra prompt, lee comandos, procesa y ejecuta
     */
    void run();
    
    /**
     * @brief Detiene la shell
     */
    void stop();
    
    /**
     * @brief Obtiene el código de salida del último comando
     * @return Código de salida
     */
    int get_last_exit_code() const;
};

#endif // SHELL_HPP
