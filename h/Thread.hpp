//
// Created by os on 6/19/24.
//

#ifndef PROJECT_BASE_THREAD_HPP
#define PROJECT_BASE_THREAD_HPP

#include "../lib/hw.h"
#include "Scheduler.hpp"
#include "MemoryAllocator.hpp"

class ThreadC;
typedef ThreadC* thread_t;

class ThreadC{
public:

    static thread_t running;

    ~ThreadC(){
        MemoryAllocator::getInstance().mem_free(stack);
    }

    using Body = void (*)(void *);

    static thread_t createThread( void * stack, Body body, void* arg) ;
    //static thread_t createThreadCPP(thread_t* handle, Body body, void* arg, void * stack);

    //static void yield();

    bool isFinished() const { return finished; }
    void setFinished(bool value) { finished = value; }

    bool isBlocked() const { return blocked; }
    void setBlocked(bool value) { blocked = value; }

    void setBody(Body body){
        this->body = body;
    }


private:

    friend class Riscv;
    friend class SemaphoreC;

    static thread_t createTCB(void * stack, Body body, void* arg);

    struct Context
    {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    void* stack;
    void* args;
    Context context;


    bool finished;
    bool blocked;


    static void threadWrapper();

    static int exit ();

    static void dispatch ();

    static void contextSwitch(Context *oldContext, Context *runningContext);
};

#endif //PROJECT_BASE_THREAD_HPP
