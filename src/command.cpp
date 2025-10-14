#include "command.hpp"
#include <iostream>
#include <cstring>

Command::Command() : append_output(false), background(false) {}

char** Command::to_argv() const {
    size_t total = args.size() + 2;
    char** argv = new char*[total];
    
    argv[0] = strdup(program.c_str());
    
    for (size_t i = 0; i < args.size(); i++) {
        argv[i + 1] = strdup(args[i].c_str());
    }
    
    argv[total - 1] = nullptr;
    
    return argv;
}

void Command::free_argv(char** argv) {
    if (argv == nullptr) return;
    
    for (int i = 0; argv[i] != nullptr; i++) {
        free(argv[i]);
    }
    delete[] argv;
}

bool Command::is_empty() const {
    return program.empty();
}

void Command::print() const {
    std::cout << "Comando: " << program;
    for (const auto& arg : args) {
        std::cout << " " << arg;
    }
    
    if (!input_file.empty()) {
        std::cout << " < " << input_file;
    }
    
    if (!output_file.empty()) {
        std::cout << (append_output ? " >> " : " > ") << output_file;
    }
    
    if (background) {
        std::cout << " &";
    }
    
    std::cout << std::endl;
}

Pipeline::Pipeline() : background(false) {}

bool Pipeline::is_empty() const {
    return commands.empty();
}

bool Pipeline::is_simple() const {
    return commands.size() == 1;
}

size_t Pipeline::size() const {
    return commands.size();
}
