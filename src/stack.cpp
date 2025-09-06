#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fmt/core.h>
#include <test_cpp/stack.hpp>

Stack::Stack(size_t elemSize) : elemSize(elemSize), maxDepth(4), currentDepth(0)
{
    stackArrPtr = std::malloc(elemSize * maxDepth);
}

Stack::~Stack()
{
    std::free(stackArrPtr);
}

void Stack::push(void* valuePtr)
{
    if (currentDepth >= maxDepth)
    {
        size_t oldSize = maxDepth * elemSize;
        maxDepth *= 2;
        size_t newSize = maxDepth  * elemSize;
        fmt::println(stdout, "Reallocating the stack array, old size: {}, new size: {}", oldSize, newSize);
        stackArrPtr = std::realloc(stackArrPtr, newSize);
    }

    std::memcpy((char*)stackArrPtr + currentDepth * elemSize, valuePtr, elemSize);
    currentDepth++;
}

void Stack::pop()
{
    currentDepth--;
}

void Stack::printStack(char* (*toString)(void* elemPtr), bool shouldFree)
{
    std::cout << "Stack content: ";
    for (size_t i = 0; i < currentDepth; i++)
    {
        auto* elemPtr = (char*)stackArrPtr + i * elemSize;
        char* elemStrPtr = toString(elemPtr);
        fmt::print(stdout, "'{}'", elemStrPtr);

        if (shouldFree)
        {
            std::free(elemStrPtr);
        }

        if (i + 1 != currentDepth)
        {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;
}
