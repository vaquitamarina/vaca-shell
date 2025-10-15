#include "builtins.hpp"
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <limits.h>
#include <errno.h>
#include <cstring>

bool Builtins::is_builtin(const std::string& cmd) const {
    return cmd == "cd" || cmd == "pwd" || cmd == "exit" || cmd == "salir" ||
        cmd == "help" || cmd == "ayuda" || cmd == "history" || cmd == "historial" ||
        cmd == "alias" || cmd == "jobs" || cmd == "trabajos";
}

int Builtins::execute_builtin(const Command& cmd) {
    const std::string& name = cmd.program;
    
    if (name == "cd") {
        return builtin_cd(cmd.args);
    }
    else if (name == "pwd") {
        return builtin_pwd(cmd.args);
    }
    else if (name == "exit" || name == "salir") {
        return builtin_exit(cmd.args);
    }
    else if (name == "help" || name == "ayuda") {
        return builtin_help(cmd.args);
    }
    else if (name == "history" || name == "historial") {
        return builtin_history(cmd.args);
    }
    else if (name == "alias") {
        return builtin_alias(cmd.args);
    }
    else if (name == "jobs" || name == "trabajos") {
        return builtin_jobs(cmd.args);
    }
    
    return 0;
}