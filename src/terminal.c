#include <termios.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static struct termios original;
static struct termios raw;
static int fd = -1;

int terminal_init() {
    fd = open("/dev/tty", O_RDWR);
    if (fd == -1) {
        fprintf(stderr, "%s: %s\n", "open", strerror(errno));
        fd = -1;
        return -1;
    }

    if (tcgetattr(fd, &original) == -1) {
        fprintf(stderr, "%s: %s\n", "tcgetattr", strerror(errno));
        close(fd);
        fd = -1;
        return -1;
    }

    raw = original;

    raw.c_lflag &= ~(ECHO | ICANON);
    raw.c_iflag &= ~(IXON | ICRNL);
    raw.c_oflag &= ~OPOST;
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;

    if (tcsetattr(fd, TCSAFLUSH, &raw) == -1) {
        fprintf(stderr, "%s: %s\n", "tcsetattr", strerror(errno));
        close(fd);
        fd = -1;
        return -1;
    }

    return 0;
}

void terminal_restore() {
    if (fd == -1)
        return;
    if (tcsetattr(fd, TCSAFLUSH, &original) == -1)
        fprintf(stderr, "%s: %s\n", "tcsetattr", strerror(errno));

    close(fd);
    fd = -1;
}
