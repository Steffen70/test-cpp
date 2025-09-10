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

static char* get_elem_ptr(const Stack* stackPtr, const size_t index)
{
    return (char*)stackPtr->stackArrPtr + index * stackPtr->elemSize;
}

static char* get_elem_ptr(const Stack* stackPtr)
{
    return get_elem_ptr(stackPtr, stackPtr->currentDepth);
}

Stack::~Stack()
{
    if (freeElem != nullptr)
    {
        for (size_t i = 0; i < currentDepth; i++)
        {
            void* currentElemPtr = get_elem_ptr(this, i);
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

    std::memcpy(get_elem_ptr(this), valuePtr, elemSize);
    currentDepth++;
}

bool Stack::pop(void* bufferPtr)
{
    if (currentDepth == 0)
        return false;

    currentDepth--;

    auto* lastElemPtr = get_elem_ptr(this);
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

static void rotate(void* frontPtr, void* middlePtr, void* endPtr)
{
    size_t frontSize = (char*)middlePtr - (char*)frontPtr;

    char buffer[frontSize];
    std::memcpy(buffer, frontPtr, frontSize);

    std::memmove(frontPtr, middlePtr, (char*)endPtr - (char*)middlePtr);

    std::memcpy((char*)endPtr - frontSize, buffer, frontSize);
}

void Stack::promote(size_t elemIndex, size_t elemCount) const
{
    rotate(get_elem_ptr(this, elemIndex), get_elem_ptr(this, elemIndex + elemCount), get_elem_ptr(this));
}

void Stack::promoteFirst(bool (*predicate)(void*)) const
{
    for (size_t i = 0; i < currentDepth; i++)
    {
        void* currentElemPtr = get_elem_ptr(this, i);
        if (predicate(currentElemPtr))
        {
            promote(i);
            return;
        }
    }
}

static void swap(void* value1Ptr, void* value2Ptr, const size_t elemSize)
{
    char buffer[elemSize];
    std::memcpy(buffer, value1Ptr, elemSize);
    std::memcpy(value1Ptr, value2Ptr, elemSize);
    std::memcpy(value2Ptr, buffer, elemSize);
}

static size_t partition(const Stack* stackPtr, const size_t startIndex, const size_t endIndex, void* (*getValuePtr)(void* elemPtr), bool (*isSmallerThan)(void* jValuePtr, void* pivotValuePtr))
{
    const size_t pivotIndex = endIndex;
    size_t i = startIndex;

    auto* pivotPtr = get_elem_ptr(stackPtr, pivotIndex);
    const auto pivotValuePtr = getValuePtr(pivotPtr);

    const auto elemSize = stackPtr->elemSize;

    for (size_t j = startIndex; j <= endIndex - 1; j++)
    {
        if (auto* jPtr = get_elem_ptr(stackPtr, j); isSmallerThan(getValuePtr(jPtr), pivotValuePtr))
        {
            swap(get_elem_ptr(stackPtr, i), jPtr, elemSize);
            i++;
        }
    }
    swap(get_elem_ptr(stackPtr, i), pivotPtr, elemSize);

    return i;
}

static void quick_sort(const Stack* stackPtr, const size_t startIndex, const size_t endIndex, void* (*getValuePtr)(void* elemPtr), bool (*isSmallerThan)(void* jValuePtr, void* pivotValuePtr))
{
    if (endIndex <= startIndex)
    {
        return;
    }

    const auto pivotIndex = partition(stackPtr, startIndex, endIndex, getValuePtr, isSmallerThan);

    if (pivotIndex > startIndex) {
        quick_sort(stackPtr, startIndex, pivotIndex - 1, getValuePtr, isSmallerThan);
    }

    if (pivotIndex < endIndex) {
        quick_sort(stackPtr, pivotIndex + 1, endIndex, getValuePtr, isSmallerThan);
    }
}

void Stack::quickSort(void* (*getValuePtr)(void* elemPtr), bool (*isSmallerThan)(void* jValuePtr, void* pivotValuePtr)) const
{
    if (currentDepth <= 1)
    {
        return;
    }

    quick_sort(this, 0, currentDepth - 1, getValuePtr, isSmallerThan);
}
