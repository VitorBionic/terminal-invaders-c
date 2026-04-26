#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <time.h>
#include "util.h"

#define FPS 60

static int sleep_time(struct timespec start, struct timespec end, struct timespec fr_time, struct timespec *diffp);
void game_request_quit(int sig);

static volatile sig_atomic_t stop = 0;

void game_loop() {
    struct timespec target_frame_time;
    struct timespec start_frame;
    struct timespec end_frame;
    struct timespec diff_frame;
    
    if (fps_to_timespec(FPS, &target_frame_time) == -1)
        game_request_quit(0);

    while (!stop) {
        if (clock_gettime(CLOCK_MONOTONIC, &start_frame) == -1)
            break;

        // input
        // render

        if (clock_gettime(CLOCK_MONOTONIC, &end_frame) == -1)
            break;

        if (sleep_time(start_frame, end_frame, target_frame_time, &diff_frame) == -1)
            break;
        nanosleep(&diff_frame, NULL);
        
    }
}

void game_request_quit(int sig) {
    (void)sig;
    stop = 1;
}

static int sleep_time(struct timespec start, struct timespec end, struct timespec fr_time, struct timespec *diffp) {
    struct timespec diff;

    if (timespec_diff(end, start, &diff) == -1)
        return -1;
    
    if (timespec_diff(fr_time, diff, diffp) == -1) {
        diffp->tv_sec = 0;
        diffp->tv_nsec = 0;
    }

    return 0;
}
