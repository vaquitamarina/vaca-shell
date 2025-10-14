#include "parser.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

std::string Parser::trim(const std::string& str) {
    size_t start = 0;
    size_t end = str.length();
    
    while (start < end && std::isspace(str[start])) {
        start++;
    }
    
    while (end > start && std::isspace(str[end - 1])) {
        end--;
    }
    
    return str.substr(start, end - start);
}

bool Parser::is_empty_or_comment(const std::string& line) {
    std::string trimmed = trim(line);
    return trimmed.empty() || trimmed[0] == '#';
}

std::vector<std::string> Parser::tokenize(const std::string& str) {
    std::vector<std::string> tokens;
    std::string current;
    bool in_quotes = false;
    bool escaped = false;
    
    for (size_t i = 0; i < str.length(); i++) {
        char c = str[i];
        
        if (escaped) {
            current += c;
            escaped = false;
            continue;
        }
        
        if (c == '\\') {
            escaped = true;
            continue;
        }
        
        if (c == '"') {
            in_quotes = !in_quotes;
            continue;
        }
        
        if (std::isspace(c) && !in_quotes) {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
            continue;
        }
        
        current += c;
    }
    
    if (!current.empty()) {
        tokens.push_back(current);
    }
    
    return tokens;
}

std::vector<std::string> Parser::split_by_pipes(const std::string& line) {
    std::vector<std::string> parts;
    std::string current;
    bool in_quotes = false;
    
    for (char c : line) {
        if (c == '"') {
            in_quotes = !in_quotes;
            current += c;
        } else if (c == '|' && !in_quotes) {
            if (!current.empty()) {
                parts.push_back(trim(current));
                current.clear();
            }
        } else {
            current += c;
        }
    }
    
    if (!current.empty()) {
        parts.push_back(trim(current));
    }
    
    return parts;
}

Command Parser::parse_command(const std::vector<std::string>& tokens) {
    Command cmd;
    
    if (tokens.empty()) {
        return cmd;
    }
    
    size_t i = 0;
    cmd.program = tokens[i++];
    
    while (i < tokens.size()) {
        const std::string& token = tokens[i];
        
        if (token == "<") {
            if (i + 1 < tokens.size()) {
                cmd.input_file = tokens[++i];
            }
            i++;
        }
        else if (token == ">>") {
            if (i + 1 < tokens.size()) {
                cmd.output_file = tokens[++i];
                cmd.append_output = true;
            }
            i++;
        }
        else if (token == ">") {
            if (i + 1 < tokens.size()) {
                cmd.output_file = tokens[++i];
                cmd.append_output = false;
            }
            i++;
        }
        else if (token == "&") {
            cmd.background = true;
            i++;
        }
        else {
            cmd.args.push_back(token);
            i++;
        }
    }
    
    return cmd;
}
