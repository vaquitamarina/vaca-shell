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

int Executor::execute_simple_command(const Command& cmd) {
    if (cmd.is_empty()) {
        return 0;
    }
    
    string program_path = resolve_path(cmd.program);
    
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("Error en fork()");
        return -1;
    }
    else if (pid == 0) {
        
        if (!setup_redirections(cmd)) {
            exit(EXIT_FAILURE);
        }
        
        char** argv = cmd.to_argv();
        
        execvp(program_path.c_str(), argv);
        
        cerr << "Error: No se pudo ejecutar '" << cmd.program << "': ";
        perror("");
        
        Command::free_argv(argv);
        exit(EXIT_FAILURE);
    }
    else {
        
        if (cmd.background) {
            cout << "[Proceso en segundo plano] PID: " << pid << endl;
            add_background_job(pid, cmd.program);
            return 0;
        }
        else {
            int status;
            pid_t result = waitpid(pid, &status, 0);
            
            if (result < 0) {
                perror("Error en waitpid()");
                return -1;
            }
            
            if (WIFEXITED(status)) {
                return WEXITSTATUS(status);
            }
            else if (WIFSIGNALED(status)) {
                cerr << "Proceso terminado por señal " 
                        << WTERMSIG(status) << endl;
                return 128 + WTERMSIG(status);
            }
        }
    }
    
    return 0;
}