//
// Created by os on 6/24/24.
//

#include "../h/syscall_cpp.hpp"

void* operator new (size_t size){
    return mem_alloc(size);
}

void operator delete (void* obj){
    mem_free(obj);
}


Thread::Thread (void (*body)(void*), void* arg){
    this->body = body;
    this->arg = arg;
}

Thread::~Thread(){
    if(myHandle){
        delete myHandle;
    }
}

int Thread::start(){
    return thread_create(&myHandle, &threadWrap, this);

}

void Thread::threadWrap(void* t){
    Thread* thread = (Thread*) t;
    if(thread->body){
        thread->body(thread->arg);
    } else {
        thread->run();
    }
}

void Thread::dispatch (){
    thread_dispatch();
}

int Thread::sleep (time_t t){
    return 0;
}

Thread::Thread (){
    this->body = nullptr;
    this->arg = nullptr;
}


//semaphore
Semaphore::Semaphore(unsigned init){
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore (){
    sem_close(myHandle);
}

int Semaphore::wait (){
    int s = sem_wait(myHandle);
    return s;
}
int Semaphore::signal(){
    int s = sem_signal(myHandle);
    return s;
}

char Console::getc (){
    return ::getc();
}
void Console::putc (char c){
    ::putc(c);
}