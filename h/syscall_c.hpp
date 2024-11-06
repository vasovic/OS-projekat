//
// Created by os on 6/17/24.
//

#ifndef PROJECT_BASE_SYSCALL_C_HPP
#define PROJECT_BASE_SYSCALL_C_HPP

#include "../lib/hw.h"
#include "Thread.hpp"
#include "Semaphore.hpp"


void* mem_alloc (size_t size);
int mem_free (void* addr);

class ThreadC;
typedef ThreadC* thread_t;
int thread_create (
        thread_t* handle,
        void(*start_routine)(void*),
        void* arg
);
int thread_exit ();
void thread_dispatch ();

class SemaphoreC;
typedef SemaphoreC* sem_t;
int sem_open (sem_t* handle, unsigned init);
int sem_close (sem_t handle);
int sem_wait (sem_t id);
int sem_signal (sem_t id);
int sem_timedwait(sem_t id, time_t timeout);
int sem_trywait(sem_t id);


typedef unsigned long time_t;
int time_sleep (time_t t);


const int EOF = -1;
char getc ();
void putc (char c);

void switch_to_user();


#endif //PROJECT_BASE_SYSCALL_C_HPP
