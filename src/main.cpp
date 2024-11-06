//
// Created by os on 6/17/24.
//
#include "../h/MemoryAllocator.hpp"
#include "../lib/console.h"
#include "../h/syscall_c.hpp"
#include "../h/riscv.hpp"




extern void userMain();

void userMainWrapper(void * arg){
    userMain();
}

void mainProba(void* args){
    __putc('o');
    __putc('k');
}

ThreadC* tmain;
ThreadC* usermain;

int main() {

    /*MemoryAllocator& allocator = MemoryAllocator::getInstance();
    void* allocatedMemory = allocator.mem_alloc(108);
    if (allocatedMemory == nullptr) {
        __putc('d');
    } else {
        __putc('n');
    }

    int freeResult = allocator.mem_free(allocatedMemory);
    if (freeResult == 0) {
        __putc('n');
    } else {
        __putc('d');
    }
    __putc('\n');

    __putc('O');
    __putc('S');
    __putc('1');
    __putc('\n');
    __putc('\n');*/

    Riscv::w_stvec((uint64) &Riscv::vectorTable & ~3ul);
    //Riscv::ms_sie(Riscv::SIE_SEIE);
    //Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    //Testiranje memorije
    /*void *alocirano = mem_alloc(100);
    if (alocirano!= nullptr) __putc('d');

    int oslobodjeno = mem_free(alocirano);
    if ( oslobodjeno == 0) {
        __putc('d');
    }*/





    thread_create(&tmain, nullptr, nullptr);
    ThreadC::running = tmain;

    switch_to_user();

    //user thread
    thread_create(&usermain, &userMainWrapper, nullptr);


    while(!usermain->isFinished()){
        thread_dispatch();
    }

    /*ThreadC* t;
    thread_create(&t, &mainProba, nullptr);
    ThreadC::running = t;

    while(!t->isFinished()){
        thread_dispatch();
    }*/

    return 0;
}