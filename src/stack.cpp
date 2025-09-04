#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fmt/core.h>
#include <test_cpp/stack.hpp>

Stack::Stack(): maxDepth(4), currentDepth(0)
{
    stackArrPtr = (int*)std::malloc(sizeof(int) * maxDepth);
}

Stack::~Stack()
{
    free(stackArrPtr);
}

void Stack::push(int value)
{
    if (currentDepth >= maxDepth)
    {
        size_t oldSize = maxDepth * sizeof(int);
        maxDepth *= 2;
        size_t newSize = sizeof(int) * maxDepth;
        fmt::println(stdout, "Reallocating the stack array, old size: {}, new size: {}", oldSize, newSize);
        auto newStackArrPtr = (int*)std::malloc(newSize);
        std::memcpy(newStackArrPtr, stackArrPtr, oldSize);
        auto oldStackArrayPtr = stackArrPtr;
        stackArrPtr = newStackArrPtr;
        std::free(oldStackArrayPtr);
    }

    std::memcpy(stackArrPtr + currentDepth, &value, sizeof(int));
    currentDepth++;
}

void Stack::pop()
{
    currentDepth--;
}

void Stack::printStack()
{
    std::cout << "Stack content: ";
    for (size_t i = 0; i < currentDepth; i++)
    {
        fmt::print(stdout, "'{}'", *(stackArrPtr + i));

        if (i + 1 != currentDepth)
        {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;
}
