//
// Created by os on 6/19/24.
//

#include "../h/Scheduler.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/Thread.hpp"

ThreadElem* Scheduler::head = nullptr;
ThreadElem* Scheduler::tail = nullptr;
//extern thread_t idle;

Scheduler& Scheduler::getInstance(){
    static Scheduler instance;
    return instance;
}

void Scheduler::put(thread_t t) {
    if(!t) return;

    size_t newSize;
    if((sizeof(ThreadElem)+sizeof(MemBlock))%MEM_BLOCK_SIZE != 0){
        newSize = (sizeof(ThreadElem) + sizeof(MemBlock))+(MEM_BLOCK_SIZE-(sizeof(ThreadElem) + sizeof(MemBlock))%MEM_BLOCK_SIZE);
    } else{
        newSize = sizeof(ThreadElem) + sizeof(MemBlock);
    }
    auto* newElem = (ThreadElem*)MemoryAllocator::getInstance().mem_alloc(newSize);

    newElem->t = t;
    newElem->next = nullptr;
    if(!head){
        head = newElem;
    } else {
        tail->next = newElem;
    }
    tail = newElem;
}

thread_t Scheduler::get() {
    if(!head){
        tail = nullptr;
        //return idle;
        return nullptr;
    }
    ThreadElem* elem = head;
    head = head->next;
    if(!head) tail = nullptr;
    elem->next = nullptr;

    thread_t t = elem->t;
    MemoryAllocator::getInstance().mem_free(elem);
    return t;
}

/*void idleThreadBody(){
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    while(true) thread_dispatch();
}*/

