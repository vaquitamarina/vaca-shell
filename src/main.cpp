#include "shell.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    
    VacaShell shell;
    
    shell.initialize();
    
    shell.run();
    
    return shell.get_last_exit_code();
}
