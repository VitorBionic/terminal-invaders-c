#include <unistd.h>
#include "terminal.h"

int main() {

    terminal_init();

    sleep(5);

    terminal_restore();
    return 0;
}
