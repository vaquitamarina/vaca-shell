/**
 * @file parser.hpp
 * @brief Parser de líneas de comando
 * 
 * Analiza la entrada del usuario y la convierte en estructuras
 * Command y Pipeline ejecutables
 */

#ifndef PARSER_HPP
#define PARSER_HPP

#include "command.hpp"
#include <string>
#include <vector>

/**
 * @class Parser
 * @brief Analizador sintáctico de comandos
 */
class Parser {
public:
    /**
     * @brief Parsea una línea completa y genera un pipeline
     * @param line Línea ingresada por el usuario
     * @return Pipeline con comandos parseados
     */
    static Pipeline parse_line(const std::string& line);
    
    /**
     * @brief Tokeniza una cadena respetando comillas
     * @param str Cadena a tokenizar
     * @return Vector de tokens
     */
    static std::vector<std::string> tokenize(const std::string& str);
    
    /**
     * @brief Parsea un comando individual (sin pipes)
     * @param tokens Tokens del comando
     * @return Comando parseado
     */
    static Command parse_command(const std::vector<std::string>& tokens);
    
    /**
     * @brief Divide una línea en comandos separados por |
     * @param line Línea completa
     * @return Vector de strings, uno por comando
     */
    static std::vector<std::string> split_by_pipes(const std::string& line);
    
    /**
     * @brief Elimina espacios al inicio y final
     * @param str Cadena a procesar
     * @return Cadena sin espacios extras
     */
    static std::string trim(const std::string& str);
    
    /**
     * @brief Verifica si una línea está vacía o es comentario
     * @param line Línea a verificar
     * @return true si está vacía o empieza con #
     */
    static bool is_empty_or_comment(const std::string& line);
};

#endif // PARSER_HPP
