# ifndef __WRAP_PTHREAD
# define __WRAP_PTHREAD

# include <pthread.h>

void Pthread_create(pthread_t* thread, const pthread_attr_t* attr, void* (*start_routine)(void*), void* arg) {
        int res = pthread_create(thread, attr, start_routine, arg);
        if (res != 0) {
                fprintf(stderr, "pthread create error");
                exit(1);
        }
}

# endif

