#include <signal.h>
#include <unistd.h>

static volatile sig_atomic_t stop = 0;
static char c;

void game_loop() {
    while (!stop) {
        if (read(STDIN_FILENO, &c, 1) > 0) {
            write(STDOUT_FILENO, &c, 1);
        }
    }
}

void game_request_quit(int sig) {
    (void)sig;
    stop = 1;
}
