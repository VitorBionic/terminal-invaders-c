#include <time.h>

#define NS_PER_SEC 1000000000L

int fps_to_timespec(unsigned int fps, struct timespec *tp) {
    if (fps == 0)
        return -1;
    double seg_per_frame = 1.0 / fps;

    unsigned int s = (unsigned int) seg_per_frame;
    seg_per_frame -= s;

    tp->tv_sec = s;
    tp->tv_nsec = seg_per_frame * NS_PER_SEC;
    return 0;
}

int timespec_diff(struct timespec after, struct timespec before, struct timespec *res) {

    long long after_nano = (NS_PER_SEC * after.tv_sec) + after.tv_nsec;
    long long before_nano = (NS_PER_SEC * before.tv_sec) + before.tv_nsec;
    long long res_nano = after_nano - before_nano;

    if (res_nano < 0)
        return -1;

    res->tv_sec = res_nano / NS_PER_SEC;
    res->tv_nsec = res_nano % NS_PER_SEC;
    
    return 0;
}
