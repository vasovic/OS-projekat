//
// Created by os on 5/6/24.
//

#include "../h/MemoryAllocator.hpp"

MemoryAllocator::MemoryAllocator() {
    freeMemHead = (MemBlock*) ((uint64)HEAP_START_ADDR);
    freeMemHead->size = (uint64) ((uint64) HEAP_END_ADDR - (uint64) HEAP_START_ADDR - sizeof(MemoryAllocator));
    freeMemHead->next = nullptr;
    freeMemHead->prev = nullptr;
}

MemoryAllocator& MemoryAllocator::getInstance(){
    static MemoryAllocator instance;
    return instance;
}

void* MemoryAllocator::mem_alloc(size_t size) {
    if(size<=0) return nullptr;

    size_t newSize = size;
    /*if((size+sizeof(MemBlock))%MEM_BLOCK_SIZE != 0){
        newSize = (size + sizeof(MemBlock))+(MEM_BLOCK_SIZE-(size + sizeof(MemBlock))%MEM_BLOCK_SIZE);
    } else{
        newSize = size + sizeof(MemBlock);
    }*/

    MemBlock* curr = freeMemHead;
    MemBlock* prev = nullptr;
    for(; curr != nullptr; prev = curr, curr = curr->next){
        if(curr->size >= newSize){
            void* addr = curr;
            size_t ostatak = curr->size - newSize;
            //ako je ostalo bar malo mesta
            if(ostatak>0){
                MemBlock* newFreeMemBlock = (MemBlock*) ((uint64)curr + newSize);

                newFreeMemBlock->prev = prev;
                newFreeMemBlock->next = curr->next;
                newFreeMemBlock->size = ostatak;

                if(prev){
                    prev->next = newFreeMemBlock;
                } else {
                    freeMemHead = newFreeMemBlock;
                }
                if(curr->next) {
                    curr->next->prev = newFreeMemBlock;
                }
            }
            //ako nije ostalo nimalo slobodnog mesta
            else{
                if(prev) prev->next = curr->next;
                else freeMemHead = curr->next;
                if(curr->next) curr->next->prev = prev;
            }

            curr->size = newSize;
            return  (char*) addr + sizeof (MemBlock);
        }
    }
    return nullptr;

}

void MemoryAllocator::tryToJoin(MemBlock *curr) {
    if(!curr) return;
    if(curr->next && (uint64) curr + curr->size == (uint64) (curr->next)){
        curr->size += curr->next->size;
        curr->next = curr->next->next;
        if(curr->next && curr->next->next){
            curr->next->next->prev = curr;
        }
    }
}

int MemoryAllocator::mem_free(void* addr) {
    if(!(uint64*)addr || (uint64*)addr > (uint64*)HEAP_END_ADDR || (uint64*)addr < (uint64*)HEAP_START_ADDR) return -1;

    MemBlock* temp = (MemBlock*) ((uint64)addr - sizeof(MemBlock));

    if(!freeMemHead){
        temp->next = nullptr;
        temp->prev = nullptr;
        freeMemHead = temp;
        return 0;
    }

    MemBlock* curr = freeMemHead;

    //trazimo mesto za segment u listi
    for(; curr < temp && curr->next != nullptr; curr = curr->next);

    //segment treba da se ubaci prvi u listu
    if(curr == freeMemHead){
        curr->prev = temp;
        temp->prev = nullptr;
        temp->next = curr;
        freeMemHead = temp;
        tryToJoin(temp);
        return 0;
    } else{
        //segment treba da se ubaci poslednji u listu
        if(temp > curr){
            curr->next = temp;
            temp->prev = curr;
            temp->next = nullptr;
            tryToJoin(curr);
            return 0;
        } else{
            temp->next = curr;
            temp->prev = curr->prev;
            curr->prev->next = temp;
            curr->prev = temp;
            tryToJoin(temp);
            tryToJoin(temp->prev);
            return 0;
        }
    }

}