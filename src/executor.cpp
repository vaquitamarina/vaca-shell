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

int Executor::execute_piped_commands(const Pipeline& pipeline) {
    size_t n = pipeline.size();
    if (n == 0) return 0;
    
    vector<pid_t> pids;
    
    vector<array<int, 2>> pipes(n - 1);
    for (size_t i = 0; i < n - 1; i++) {
        if (pipe(pipes[i].data()) < 0) {
            perror("Error al crear pipe");
            return -1;
        }
    }
    
    for (size_t i = 0; i < n; i++) {
        const Command& cmd = pipeline.commands[i];
        string program_path = resolve_path(cmd.program);
        
        pid_t pid = fork();
        
        if (pid < 0) {
            perror("Error en fork (pipeline)");
            return -1;
        }
        else if (pid == 0) {
            
            if (i > 0) {
                if (dup2(pipes[i-1][0], STDIN_FILENO) < 0) {
                    perror("Error en dup2 (stdin pipe)");
                    exit(EXIT_FAILURE);
                }
            }
            
            if (i < n - 1) {
                if (dup2(pipes[i][1], STDOUT_FILENO) < 0) {
                    perror("Error en dup2 (stdout pipe)");
                    exit(EXIT_FAILURE);
                }
            }
            
            for (size_t j = 0; j < n - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            
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
            pids.push_back(pid);
        }
    }
    
    for (size_t i = 0; i < n - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
    
    int last_status = 0;
    
    if (pipeline.background) {
        cout << "[Pipeline en segundo plano] PIDs:";
        for (pid_t pid : pids) {
            cout << " " << pid;
            add_background_job(pid, "pipeline");
        }
        cout << endl;
    }
    else {
        for (pid_t pid : pids) {
            int status;
            if (waitpid(pid, &status, 0) < 0) {
                perror("Error en waitpid (pipeline)");
            }
            else if (WIFEXITED(status)) {
                last_status = WEXITSTATUS(status);
            }
        }
    }
    
    return last_status;
}

int Executor::execute_pipeline(const Pipeline& pipeline) {
    if (pipeline.is_empty()) {
        return 0;
    }
    
    collect_background_jobs();
    
    if (pipeline.is_simple()) {
        return execute_simple_command(pipeline.commands[0]);
    }
    else {
        return execute_piped_commands(pipeline);
    }
}

void Executor::add_background_job(pid_t pid, const string& command) {
    background_jobs[pid] = command;
}

void Executor::collect_background_jobs() {
    auto it = background_jobs.begin();
    while (it != background_jobs.end()) {
        pid_t pid = it->first;
        int status;
        
        pid_t result = waitpid(pid, &status, WNOHANG);
        
        if (result > 0) {

            if (WIFEXITED(status)) {
                cout << "\n[Proceso " << pid << " (" << it->second 
                        << ") terminó con código " << WEXITSTATUS(status) 
                        << "]" << endl;
            }
            else if (WIFSIGNALED(status)) {
                cout << "\n[Proceso " << pid << " (" << it->second 
                        << ") terminado por señal " << WTERMSIG(status) 
                        << "]" << endl;
            }
            
            it = background_jobs.erase(it);
        }
        else if (result == 0) {
            ++it;
        }
        else {
            ++it;
        }
    }
}

void Executor::show_jobs() const {
    if (background_jobs.empty()) {
        cout << "No hay trabajos en segundo plano." << endl;
    }
    else {
        cout << "Trabajos en segundo plano:" << endl;
        for (const auto& job : background_jobs) {
            cout << "  [" << job.first << "] " << job.second << endl;
        }
    }
}