#pragma once
#include <cstddef>

struct Stack
{
    std::size_t elemSize;
    std::size_t maxDepth;
    std::size_t currentDepth;
    void* stackArrPtr;
    Stack(std::size_t elemSize);
    ~Stack();
    void push(const void* valuePtr);
    void* pop();
    void printStack(char* (*toString)(void* elemPtr), bool shouldFree = false);
};
