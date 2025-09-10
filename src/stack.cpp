#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fmt/core.h>
#include <test_cpp/stack.hpp>

Stack::Stack(const size_t elemSize, void (*freeElem)(void*)) : elemSize(elemSize), maxDepth(4), currentDepth(0), freeElem(freeElem)
{
    stackArrPtr = std::malloc(elemSize * maxDepth);
}

Stack::~Stack()
{
    if (freeElem != nullptr)
    {
        for (size_t i = 0; i < currentDepth; i++)
        {
            void* currentElemPtr = (char*)stackArrPtr + i * elemSize;
            freeElem(currentElemPtr);
        }
    }
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

bool Stack::pop(void* bufferPtr)
{
    if (currentDepth == 0)
        return false;

    currentDepth--;

    auto* lastElemPtr = (char*)stackArrPtr + currentDepth * elemSize;
    std::memcpy(bufferPtr, lastElemPtr, elemSize);
    return true;
}

static void print_stack(Stack* stackPtr, const bool shouldFree, const bool isRecursion, char* (*toString)(void* elemPtr), char* (*toStringWithFreeElem)(void* elemPtr, void (*freeElem)(void*)))
{
    if (!isRecursion)
    {
        std::cout << "Stack content:" << std::endl;
    }

    auto* bufferPtr = std::malloc(stackPtr->elemSize);

    if (!stackPtr->pop(bufferPtr))
    {
        return;
    }

    char* elemStrPtr = nullptr;
    if (toString != nullptr)
    {
        elemStrPtr = toString(bufferPtr);
    }
    else
    {
        elemStrPtr = toStringWithFreeElem(bufferPtr, stackPtr->freeElem);
    }

    fmt::println(stdout, "'{}',", elemStrPtr);
    std::free(bufferPtr);
    if (shouldFree)
    {
        std::free(elemStrPtr);
    }

    print_stack(stackPtr, shouldFree, true, toString, toStringWithFreeElem);
}

void Stack::printStack(char* (*toString)(void* elemPtr), const bool shouldFree)
{
    print_stack(this, shouldFree, false, toString, nullptr);
}

void Stack::printStack(char*(*toString)(void* elemPtr, void (*freeElem)(void*)), bool shouldFree)
{
    print_stack(this, shouldFree, false, nullptr, toString);
}

void Stack::promote(size_t elemIndex, size_t elemCount)
{
    auto* frontPtr = (char*)stackArrPtr + elemIndex * elemSize;
    size_t bufferSize = elemCount * elemSize;
    auto* middlePtr = frontPtr + bufferSize;
    auto* newEndPtr = (char*)stackArrPtr + currentDepth * elemSize - bufferSize;

    char buffer[bufferSize];
    std::memcpy(buffer, frontPtr, bufferSize);

    std::memmove(frontPtr, middlePtr, (currentDepth - (elemIndex + elemCount)) * elemSize);

    std::memcpy(newEndPtr, buffer, bufferSize);
}

void Stack::promoteFirst(bool (*predicate)(void*))
{
    for (size_t i = 0; i < currentDepth; i++)
    {
        void* currentElemPtr = (char*)stackArrPtr + i * elemSize;
        if (predicate(currentElemPtr))
        {
            promote(i);
            return;
        }
    }
}
