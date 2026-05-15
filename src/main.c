#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include "terminal.h"
#include "game.h"

void setup_signal_handlers(void);

int main() {

    srand((unsigned int) time(NULL));

    terminal_init();

    setup_signal_handlers();

    game_loop();

    terminal_restore();
    return 0;
}

void setup_signal_handlers() {
    signal(SIGINT, game_request_quit);
    signal(SIGTERM, game_request_quit);
}
