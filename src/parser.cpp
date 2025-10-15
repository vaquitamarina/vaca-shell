#include "parser.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

string Parser::trim(const string& str) {
    size_t start = 0;
    size_t end = str.length();
    
    while (start < end && isspace(str[start])) {
        start++;
    }
    
    while (end > start && isspace(str[end - 1])) {
        end--;
    }
    
    return str.substr(start, end - start);
}

bool Parser::is_empty_or_comment(const string& line) {
    string trimmed = trim(line);
    return trimmed.empty() || trimmed[0] == '#';
}

vector<string> Parser::tokenize(const string& str) {
    vector<string> tokens;
    string current;
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
        
        if (isspace(c) && !in_quotes) {
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

vector<string> Parser::split_by_pipes(const string& line) {
    vector<string> parts;
    string current;
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

Command Parser::parse_command(const vector<string>& tokens) {
    Command cmd;
    
    if (tokens.empty()) {
        return cmd;
    }
    
    size_t i = 0;
    cmd.program = tokens[i++];
    
    while (i < tokens.size()) {
        const string& token = tokens[i];
        
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

Pipeline Parser::parse_line(const string& line) {
    Pipeline pipeline;
    
    if (is_empty_or_comment(line)) {
        return pipeline;
    }
    
    vector<string> parts = split_by_pipes(line);
    
    bool has_background = false;
    if (!parts.empty()) {
        string& last_part = parts.back();
        size_t amp_pos = last_part.find('&');
        if (amp_pos != string::npos) {
            has_background = true;
            last_part = trim(last_part.substr(0, amp_pos));
        }
    }
    
    for (const auto& part : parts) {
        vector<string> tokens = tokenize(part);
        if (!tokens.empty()) {
            Command cmd = parse_command(tokens);
            if (!cmd.is_empty()) {
                pipeline.commands.push_back(cmd);
            }
        }
    }
    
    if (has_background && !pipeline.commands.empty()) {
        pipeline.background = true;
        pipeline.commands.back().background = true;
    }
    
    return pipeline;
}
