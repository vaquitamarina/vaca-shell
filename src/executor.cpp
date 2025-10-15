#include "executor.hpp"
#include <iostream>
#include <array>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <errno.h>

using namespace std;

string Executor::resolve_path(const string& program) {
    if (program.find('/') != string::npos) {
        return program;
    }
    
    string bin_path = "/bin/" + program;
    if (access(bin_path.c_str(), X_OK) == 0) {
        return bin_path;
    }
    
    string usr_bin_path = "/usr/bin/" + program;
    if (access(usr_bin_path.c_str(), X_OK) == 0) {
        return usr_bin_path;
    }
    
    return program;
}

bool Executor::setup_redirections(const Command& cmd) {
    if (!cmd.input_file.empty()) {
        int fd = open(cmd.input_file.c_str(), O_RDONLY);
        if (fd < 0) {
            perror(("Error al abrir '" + cmd.input_file + "'").c_str());
            return false;
        }
        
        if (dup2(fd, STDIN_FILENO) < 0) {
            perror("Error en dup2 (entrada)");
            close(fd);
            return false;
        }
        
        close(fd);
    }
    
    if (!cmd.output_file.empty()) {
        int flags = O_WRONLY | O_CREAT;
        flags |= cmd.append_output ? O_APPEND : O_TRUNC;
        
        int fd = open(cmd.output_file.c_str(), flags, 0644);
        if (fd < 0) {
            perror(("Error al abrir '" + cmd.output_file + "'").c_str());
            return false;
        }
        
        if (dup2(fd, STDOUT_FILENO) < 0) {
            perror("Error en dup2 (salida)");
            close(fd);
            return false;
        }
        
        close(fd);
    }
    
    return true;
}