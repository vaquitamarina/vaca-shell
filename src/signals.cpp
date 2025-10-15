#include "signals.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <cerrno> 


volatile sig_atomic_t g_child_finished = 0;
static volatile pid_t foreground_pid = 0;

void SignalHandler::sigint_handler(int signo) {
    (void)signo;
    
    if (foreground_pid > 0) {
        kill(foreground_pid, SIGINT);
    }
    else {
        write(STDOUT_FILENO, "\n", 1);
    }
}

void SignalHandler::sigchld_handler(int signo) {
    (void)signo;
    
    g_child_finished = 1;
}

void SignalHandler::setup_signals() {
    struct sigaction sa_int;
    
    memset(&sa_int, 0, sizeof(sa_int));
    sa_int.sa_handler = sigint_handler;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = SA_RESTART;
    
    if (sigaction(SIGINT, &sa_int, nullptr) < 0) {
        perror("Error al configurar SIGINT");
    }
    
    struct sigaction sa_chld;
    memset(&sa_chld, 0, sizeof(sa_chld));
    sa_chld.sa_handler = sigchld_handler;
    sigemptyset(&sa_chld.sa_mask);
    sa_chld.sa_flags = SA_RESTART | SA_NOCLDSTOP;

    if (sigaction(SIGCHLD, &sa_chld, nullptr) < 0) {
        perror("Error al configurar SIGCHLD");
    }

    signal(SIGTSTP, SIG_IGN);
}

void SignalHandler::restore_default_handlers() {
    struct sigaction sa_default;
    memset(&sa_default, 0, sizeof(sa_default));
    sa_default.sa_handler = SIG_DFL;
    sigemptyset(&sa_default.sa_mask);
    
    sigaction(SIGINT, &sa_default, nullptr);
    sigaction(SIGCHLD, &sa_default, nullptr);
    sigaction(SIGTSTP, &sa_default, nullptr);
}

void SignalHandler::ignore_sigint() {
    struct sigaction sa_ignore;
    memset(&sa_ignore, 0, sizeof(sa_ignore));
    sa_ignore.sa_handler = SIG_IGN;
    sigemptyset(&sa_ignore.sa_mask);
    
    sigaction(SIGINT, &sa_ignore, nullptr);
}

void SignalHandler::restore_sigint() {
    setup_signals();
}