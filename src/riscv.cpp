//
// Created by os on 6/18/24.
//

#include "../h/riscv.hpp"
#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/Thread.hpp"
#include "../h/Semaphore.hpp"

using Body = void (*)(void*);

void Riscv::popSppSpie()
{
    __asm__ volatile ("csrw sepc, ra");
    __asm__ volatile ("sret");
}


void Riscv::handleSupervisorTrap() {

    uint64 volatile scause = r_scause();

    uint64 volatile arg1, arg2, arg3, arg4;
    __asm__ volatile("mv %[arg4], a4" : [arg4] "=r" (arg4));
    __asm__ volatile("mv %[arg3], a3" : [arg3] "=r" (arg3));
    __asm__ volatile("mv %[arg2], a2" : [arg2] "=r" (arg2));
    __asm__ volatile("mv %[arg1], a1" : [arg1] "=r" (arg1));

    uint64  oc;
    __asm__ volatile("mv %[oc], a0" : [oc] "=r" (oc));

    if(scause == 0x08 || scause == 0x09){
        uint64 volatile sepc = r_sepc() + 4;
        uint64 volatile sstatus = r_sstatus();

        switch (oc) {
            case mem_alloc:{
                uint64 size = arg1;
                void * ret = MemoryAllocator::getInstance().mem_alloc(size*MEM_BLOCK_SIZE);
                __asm__ volatile ("mv a0, %[ret]" : : [ret]"r" (ret));
                break;
            }
            case mem_free: {
                void *addr = (void*) arg1;
                int ret = MemoryAllocator::getInstance().mem_free(addr);
                __asm__ volatile ("mv a0, %[ret]" : : [ret]"r"(ret));
                break;
            }
            case thread_create:{
                thread_t* handle = (thread_t*)arg1;
                Body body= (Body) arg2;
                void* arg = (void*) arg3;
                void * stack_space = (void *) arg4;

                *handle = ThreadC::createThread( stack_space, body, arg);

                int ret = 0;
                if(*handle == nullptr) ret = -1;
                __asm__ volatile ("mv a0, %[ret]" : : [ret]"r" (ret));
                break;
            }
            case thread_exit: {
                int ret = ThreadC::exit();
                __asm__ volatile ("mv a0, %[ret]" : : [ret]"r" (ret));
                break;
            }
            case thread_dispatch:{
                ThreadC::dispatch();
                break;
            }
            case sem_open:{
                sem_t* handle = (sem_t*) arg1;
                unsigned init = (unsigned) arg2;

                *handle = SemaphoreC::semOpen(init);

                int ret = 0;
                if(*handle == nullptr) ret = -1;
                __asm__ volatile ("mv a0, %[ret]" : : [ret]"r" (ret));

                break;
            }
            case sem_close:{
                sem_t handle = (sem_t) arg1;
                int ret;
                if(handle){
                    ret = handle->semClose();
                }
                else ret = -1;

                __asm__ volatile ("mv a0, %[ret]" : : [ret]"r" (ret));

                break;
            }
            case sem_wait:{
                sem_t handle = (sem_t) arg1;
                int ret;
                if(handle){
                    ret = handle->semWait();
                }
                else ret = -1;

                __asm__ volatile ("mv a0, %[ret]" : : [ret]"r" (ret));

                break;
            }
            case sem_signal:{
                sem_t handle = (sem_t) arg1;

                int ret;
                if(!handle) ret = -1;
                else ret = handle->semSignal();

                __asm__ volatile ("mv a0, %[ret]" : : [ret]"r" (ret));

                break;
            }
            case sem_trywait:{
                sem_t handle = (sem_t) arg1;

                int ret;
                if(!handle) ret = -1;
                else ret = handle->semTryWait();

                __asm__ volatile ("mv a0, %[ret]" : : [ret]"r" (ret));

                break;
            }
            case getc:{
                char c = __getc();
                __asm__ volatile ("mv a0, %[c]" : : [c]"r" (c));

                break;
            }
            case putc: {
                char c = (char)arg1;
                __putc(c);

                break;
            }
            case switch_user: {
                mc_sip(SIP_SSIP);
                w_sstatus(sstatus);
                mc_sstatus(SSTATUS_SPP);
                w_sepc(sepc);
                return;
            }
        }

        w_sepc(sepc);
        w_sstatus(sstatus);

    }
    else if(scause == 0x8000000000000001UL){
        mc_sip(SIP_SSIP);
    }
    else if(scause == 0x8000000000000009UL){
        console_handler();
    }
    else{
        // unexpected trap cause
        __putc(scause%16+'0');
    }

}