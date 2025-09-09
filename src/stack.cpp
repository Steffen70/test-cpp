#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fmt/core.h>
#include <test_cpp/stack.hpp>

Stack::Stack(const size_t elemSize) : elemSize(elemSize), maxDepth(4), currentDepth(0)
{
    stackArrPtr = std::malloc(elemSize * maxDepth);
}

Stack::~Stack()
{
    std::free(stackArrPtr);
}

void Stack::push(const void* valuePtr)
{
    if (currentDepth >= maxDepth)
    {
        size_t oldSize = maxDepth * elemSize;
        maxDepth *= 2;
        size_t newSize = maxDepth * elemSize;
        fmt::println(stdout, "Reallocating the stack array, old size: {}, new size: {}", oldSize, newSize);
        stackArrPtr = std::realloc(stackArrPtr, newSize);
    }

    std::memcpy((char*)stackArrPtr + currentDepth * elemSize, valuePtr, elemSize);
    currentDepth++;
}

void* Stack::pop()
{
    currentDepth--;
    if (currentDepth == -1)
        return nullptr;

    auto* lastElemPtr = (char*)stackArrPtr + currentDepth * elemSize;
    return lastElemPtr;
}

static void print_stack(Stack* stackPtr, char* (*toString)(void* elemPtr), const bool shouldFree, const bool isRecursion)
{
    if (!isRecursion)
    {
        std::cout << "Stack content:" << std::endl;
    }

    auto* currentElemPtr = stackPtr->pop();

    if (currentElemPtr == nullptr)
    {
        return;
    }

    char* elemStrPtr = toString(currentElemPtr);
    fmt::println(stdout, "'{}',", elemStrPtr);
    if (shouldFree)
    {
        std::free(elemStrPtr);
    }

    print_stack(stackPtr, toString, shouldFree, true);
}

void Stack::printStack(char* (*toString)(void* elemPtr), const bool shouldFree)
{
    print_stack(this, toString, shouldFree, false);
}