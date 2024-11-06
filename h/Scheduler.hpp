//
// Created by os on 6/19/24.
//

#ifndef PROJECT_BASE_SCHEDULER_HPP
#define PROJECT_BASE_SCHEDULER_HPP


class ThreadC;
typedef ThreadC* thread_t;

struct ThreadElem {
    thread_t t;
    ThreadElem* next;
};

class Scheduler {

public:
    static Scheduler& getInstance();

    static void put(thread_t t);
    static thread_t get();

private:
    static ThreadElem* head;
    static ThreadElem* tail;

};

#endif //PROJECT_BASE_SCHEDULER_HPP
