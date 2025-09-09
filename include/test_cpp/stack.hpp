#pragma once
#include <cstddef>

struct Stack
{
    std::size_t elemSize;
    std::size_t maxDepth;
    std::size_t currentDepth;
    void* stackArrPtr;
    void (*freeElem)(void*);
    Stack(std::size_t elemSize, void (*freeElem)(void*) = nullptr);
    ~Stack();
    void push(const void* valuePtr);
    bool pop(void* bufferPtr);
    void printStack(char* (*toString)(void* elemPtr), bool shouldFree = false);
    void printStack(char* (*toString)(void* elemPtr, void (*freeElem)(void*)), bool shouldFree = true);
};
