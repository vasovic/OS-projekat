//
// Created by os on 6/19/24.
//

#include "../h/Thread.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.hpp"

thread_t ThreadC::running = nullptr;

thread_t ThreadC::createTCB(void *stack, ThreadC::Body body, void *arg) {
    size_t size;
    if((sizeof(ThreadC)+sizeof(MemBlock))%MEM_BLOCK_SIZE != 0){
        size = (sizeof(ThreadC) + sizeof(MemBlock)) + (MEM_BLOCK_SIZE - (sizeof(ThreadC) + sizeof(MemBlock)) % MEM_BLOCK_SIZE);
    } else{
        size = sizeof(ThreadC) + sizeof(MemBlock);
    }

    thread_t newT = (thread_t) MemoryAllocator::getInstance().mem_alloc(size);
    newT->setBody(body);
    if(body == nullptr) newT->stack = nullptr;
    else newT->stack = stack;
    newT->args = arg;
    newT->finished = false;
    newT->blocked = false;
    newT->context.ra = (uint64) &threadWrapper;
    newT->context.sp = newT->stack != nullptr ? (uint64) newT->stack+DEFAULT_STACK_SIZE : 0;
    return newT;

}

thread_t ThreadC::createThread( void *stack, Body body, void * arguments){
    thread_t t = createTCB( stack, body, arguments);

    if (body != nullptr){
        Scheduler::put(t);
    }
    return t;
}

/*thread_t ThreadC::createThreadCPP(thread_t *handle, ThreadC::Body body, void *arg, void *stack) {
    thread_t t = createTCB(stack, body, arg);
    *handle = t;
    return t;
}*/

/*void ThreadC::yield(){
    uint64 oc = 0x13;
    __asm__ volatile ("mv a0, %[oc]" : : [oc] "r" (oc));
    __asm__ volatile ("ecall");
}*/

void ThreadC::dispatch() {
    thread_t old = running;
    if(old->isFinished()==false && old->isBlocked()==false){
        Scheduler::put(old);
    }
    running = Scheduler::get();
    contextSwitch(&old->context, &running->context);
}

void ThreadC::threadWrapper() {
    //Riscv::popSppSpie(); //upisuje vrednost ra u pc
    switch_to_user();
    running->body(running->args);
    thread_exit();
}

int ThreadC::exit() {
    if(!running->isFinished()){
        running->setFinished(true);
        //MemoryAllocator::getInstance().mem_free(running);
        thread_dispatch();
        return 0;
    }
    return -1;
}


