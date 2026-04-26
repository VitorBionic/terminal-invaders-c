#ifndef UTIL_H
#define UTIL_H

int fps_to_timespec(unsigned int fps, struct timespec *tp);
int timespec_diff(struct timespec after, struct timespec before, struct timespec *res);

#endif
