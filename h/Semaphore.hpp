//
// Created by os on 6/24/24.
//

#ifndef PROJECT_BASE_SEMAPHORE_HPP
#define PROJECT_BASE_SEMAPHORE_HPP

#include "Thread.hpp"
#include "Scheduler.hpp"



class SemaphoreC;
typedef SemaphoreC* sem_t;

class SemaphoreC{

public:
    static SemaphoreC* semOpen(int val);
    int semClose();
    int semWait();
    int semTryWait();
    int semSignal();

private:
    int val;
    ThreadElem* blockedHead;
    ThreadElem* blockedTail;

};
#endif //PROJECT_BASE_SEMAPHORE_HPP
