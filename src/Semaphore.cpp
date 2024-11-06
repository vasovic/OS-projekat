//
// Created by os on 6/24/24.
//

#include "../h/Semaphore.hpp"
#include "../h/riscv.hpp"

SemaphoreC* SemaphoreC::semOpen(int val) {

    size_t size= (sizeof(SemaphoreC) + sizeof(MemBlock)) + (MEM_BLOCK_SIZE - (sizeof(SemaphoreC) + sizeof(MemBlock)) % MEM_BLOCK_SIZE);

    SemaphoreC* newS = (SemaphoreC*) MemoryAllocator::getInstance().mem_alloc(size);
    newS->val = val;
    newS->blockedHead = nullptr;
    newS->blockedTail = nullptr;
    return newS;
}

int SemaphoreC::semSignal() {
    val++;
    if(val <= 0){
        ThreadElem* elem = blockedHead;
        blockedHead = blockedHead->next;
        elem->t->setBlocked(false);
        Scheduler::put(elem->t);
        if(!blockedHead) blockedTail = nullptr;
        MemoryAllocator::getInstance().mem_free(elem);
    }
    return 0;
}

int SemaphoreC::semWait() {
    val--;
    if(val < 0){
        ThreadC::running->setBlocked(true);

        size_t size = (sizeof(ThreadElem) + sizeof(MemBlock)) + (MEM_BLOCK_SIZE - (sizeof(ThreadElem) + sizeof(MemBlock)) % MEM_BLOCK_SIZE);
        ThreadElem* elem = (ThreadElem*) MemoryAllocator::getInstance().mem_alloc(size);

        elem->t = ThreadC::running;
        elem->next = nullptr;

        if(!blockedHead) blockedHead = elem;
        else blockedTail->next = elem;
        blockedTail = elem;
        ThreadC::dispatch();
    }
    return 0;
}

int SemaphoreC::semTryWait() {
    val--;
    if(val < 0) {
        ThreadC::dispatch();
        return -1;
    }
    else if(val == 0) {
        return 0;
        } else return 1;

}

int SemaphoreC::semClose() {
    ThreadElem* curr;
    curr = blockedHead;

    while(curr){
        curr->t->setBlocked(false);
        Scheduler::put(curr->t);
        ThreadElem* prev = curr;
        curr = curr->next;
        MemoryAllocator::getInstance().mem_free(prev);
    }

    blockedHead = nullptr;
    blockedTail = nullptr;
    MemoryAllocator::getInstance().mem_free(curr);
    return 0;

}