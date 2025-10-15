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