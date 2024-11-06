//
// Created by os on 6/17/24.
//

#include "../h/syscall_c.hpp"

void* mem_alloc(size_t size){
    uint64 oc = 0x01;
    size_t sizeBlock = (size+ sizeof(MemBlock)) % MEM_BLOCK_SIZE == 0 ?
                       (size+ sizeof(MemBlock))/MEM_BLOCK_SIZE : (size+ sizeof(MemBlock))/MEM_BLOCK_SIZE + 1;

    __asm__ volatile ("mv a1,%[size]" : :[size]"r" (sizeBlock));
    __asm__ volatile ("mv a0, %[oc]" : : [oc]"r" (oc));

    __asm__ volatile ("ecall");

    uint64 ret;
    __asm__ volatile ("mv %[ret], a0" : [ret]"=r" (ret));
    return (void*) ret;
}

int mem_free (void* addr){
    uint64 oc = 0x02;

    __asm__ volatile ("mv a1, %[address]" : : [address]"r" (addr));
    __asm__ volatile ("mv a0, %[oc]" : : [oc]"r" (oc));

    __asm__ volatile ("ecall");

    int ret;
    __asm__ volatile ("mv %[ret], a0" : [ret]"=r" (ret));
    return ret;
}

int thread_create (
        thread_t* handle,
        void(*start_routine)(void*),
        void* arg
){

    uint64 oc = 0x11;

    void * stack_space = nullptr;
    if (start_routine) {
        stack_space = mem_alloc(DEFAULT_STACK_SIZE);
        if (stack_space == nullptr) {
            return -1;
        }
    }

    __asm__ volatile ("mv a4, %[stack_space]" : : [stack_space] "r" (stack_space));
    __asm__ volatile ("mv a3, %[argument]" : : [argument] "r" (arg));
    __asm__ volatile ("mv a2, %[sr]" : : [sr] "r" (start_routine));
    __asm__ volatile ("mv a1, %[handle]" : : [handle] "r" (handle));
    __asm__ volatile ("mv a0, %[oc]" : : [oc] "r" (oc));

    __asm__ volatile ("ecall");

    int ret;
    __asm__ volatile ("mv %[ret], a0" : [ret]"=r" (ret));
    return ret;
}

int thread_exit(){

    uint64 oc = 0x12;

    __asm__ volatile("mv a0, %[oc]" : : [oc]"r" (oc));

    __asm__ volatile("ecall");

    int ret;
    __asm__ volatile ("mv %[ret], a0" : [ret]"=r" (ret));
    return ret;
};

void thread_dispatch(){

    uint64 oc = 0x13;

    __asm__ volatile("mv a0, %[oc]" : : [oc]"r" (oc));

    __asm__ volatile("ecall");
}

int sem_open (sem_t* handle, unsigned init){

    uint64 oc = 0x21;

    __asm__ volatile ("mv a2, %[init]" : : [init] "r" (init));
    __asm__ volatile ("mv a1, %[handle]" : : [handle] "r" (handle));
    __asm__ volatile ("mv a0, %[oc]" : : [oc]"r" (oc));

    __asm__ volatile ("ecall");

    int ret;
    __asm__ volatile ("mv %[ret], a0" : [ret]"=r" (ret));
    return ret;
}

int sem_close (sem_t handle){

    uint64 oc = 0x22;

    __asm__ volatile ("mv a1, %[handle]" : : [handle] "r" (handle));
    __asm__ volatile ("mv a0, %[oc]" : : [oc]"r" (oc));

    __asm__ volatile ("ecall");

    int ret;
    __asm__ volatile ("mv %[ret], a0" : [ret]"=r" (ret));
    return ret;
}

int sem_wait (sem_t id){

    uint64 oc = 0x23;

    __asm__ volatile ("mv a1, %[handle]" : : [handle] "r" (id));
    __asm__ volatile ("mv a0, %[oc]" : : [oc]"r" (oc));

    __asm__ volatile ("ecall");

    int ret;
    __asm__ volatile ("mv %[ret], a0" : [ret]"=r" (ret));
    return ret;
}

int sem_signal (sem_t id){

    uint64 oc = 0x24;

    __asm__ volatile ("mv a1, %[handle]" : : [handle] "r" (id));
    __asm__ volatile ("mv a0, %[oc]" : : [oc]"r" (oc));

    __asm__ volatile ("ecall");

    int ret;
    __asm__ volatile ("mv %[ret], a0" : [ret]"=r" (ret));
    return ret;
}

int sem_timedwait (sem_t id, time_t timeout){
    return 0;
}

int sem_trywait (sem_t id){

    uint64 oc = 0x26;

    __asm__ volatile ("mv a1, %[handle]" : : [handle] "r" (id));
    __asm__ volatile ("mv a0, %[oc]" : : [oc]"r" (oc));

    __asm__ volatile ("ecall");

    int ret;
    __asm__ volatile ("mv %[ret], a0" : [ret]"=r" (ret));
    return ret;
}

int time_sleep(time_t time) {
    return 0;
}

char getc() {

    uint64 oc = 0x41;

    __asm__ volatile ("mv a0, %[oc]" : : [oc]"r" (oc));

    __asm__ volatile ("ecall");

    int ret;
    __asm__ volatile ("mv %[ret], a0" : [ret]"=r" (ret));
    return ret;
}

void putc(char c){

    uint64 oc = 0x42;

    __asm__ volatile ("mv a1, %[character]" : : [character] "r" (c));
    __asm__ volatile ("mv a0, %[oc]" : : [oc]"r" (oc));

    __asm__ volatile ("ecall");

}

void switch_to_user(){

    uint64 oc = 0x43;

    __asm__ volatile ("mv a0, %[oc]" : : [oc]"r" (oc));

    __asm__ volatile ("ecall");
}
