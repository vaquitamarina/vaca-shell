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
