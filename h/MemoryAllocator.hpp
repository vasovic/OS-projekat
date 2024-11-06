//
// Created by os on 5/6/24.
//

#ifndef PROJECT_BASE_MEMORYALLOCATOR_HPP
#define PROJECT_BASE_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"

struct MemBlock{
    MemBlock* prev;
    MemBlock* next;
    size_t size;
};

class MemoryAllocator {
public:
    static MemoryAllocator& getInstance();
    void* mem_alloc(size_t size);
    int mem_free(void* addr);

private:
    MemoryAllocator();
    MemBlock* freeMemHead = nullptr;
    void tryToJoin(MemBlock* curr);

};

#endif //PROJECT_BASE_MEMORYALLOCATOR_HPP
