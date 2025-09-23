#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stack/stack.h>

void stack_init(Stack* s, const size_t elemSize, const free_elem freeElem)
{
    s->elemSize = elemSize;
    s->maxDepth = 4;
    s->currentDepth = 0;
    s->freeElem = freeElem;
    s->stackArrPtr = malloc(elemSize * s->maxDepth);
}

static char* get_elem_ptr(const Stack* s, const size_t index)
{
    return (char*)s->stackArrPtr + index * s->elemSize;
}

static char* get_elem_ptr_simplified(const Stack* s)
{
    return get_elem_ptr(s, s->currentDepth);
}

void stack_destroy(const Stack* s)
{
    if (s->freeElem != nullptr)
    {
        for (size_t i = 0; i < s->currentDepth; i++)
        {
            void* currentElemPtr = get_elem_ptr(s, i);
            s->freeElem(currentElemPtr);
        }
    }
    free(s->stackArrPtr);
}

void stack_push(Stack* s, const void* valuePtr)
{
    if (s->currentDepth >= s->maxDepth)
    {
        const size_t oldSize = s->maxDepth * s->elemSize;
        s->maxDepth *= 2;
        const size_t newSize = s->maxDepth * s->elemSize;
        fprintf(stdout, "Reallocating the stack array, old size: %zu, new size: %zu\n", oldSize, newSize);
        void* newArrPtr = realloc(s->stackArrPtr, newSize);
        assert(newArrPtr != nullptr);
        s->stackArrPtr = newArrPtr;
    }

    memcpy(get_elem_ptr_simplified(s), valuePtr, s->elemSize);
    s->currentDepth++;
}

bool stack_pop(Stack* s, void* bufferPtr)
{
    if (s->currentDepth == 0)
        return false;

    s->currentDepth--;

    const char* lastElemPtr = get_elem_ptr_simplified(s);
    memcpy(bufferPtr, lastElemPtr, s->elemSize);
    return true;
}

static void print_stack(Stack* s, const bool shouldFree, const bool isRecursion, const to_string toString, const to_string_extended toStringWithFreeElem)
{
    if (!isRecursion)
    {
        printf("Stack content:\n");
    }

    void* bufferPtr = malloc(s->elemSize);

    if (!stack_pop(s, bufferPtr))
    {
        return;
    }

    char* elemStrPtr;
    if (toString != nullptr)
    {
        elemStrPtr = toString(bufferPtr);
    }
    else
    {
        elemStrPtr = toStringWithFreeElem(bufferPtr, s->freeElem);
    }

    fprintf(stdout, "'%s',\n", elemStrPtr);
    free(bufferPtr);
    if (shouldFree)
    {
        free(elemStrPtr);
    }

    print_stack(s, shouldFree, true, toString, toStringWithFreeElem);
}

void stack_print(Stack* s, const to_string toString, const bool shouldFree)
{
    print_stack(s, shouldFree, false, toString, nullptr);
}

void stack_print_extended(Stack* s, const to_string_extended toString, const bool shouldFree)
{
    print_stack(s, shouldFree, false, nullptr, toString);
}

static void rotate(void* frontPtr, void* middlePtr, void* endPtr)
{
    const size_t frontSize = (char*)middlePtr - (char*)frontPtr;

    char buffer[frontSize];
    memcpy(buffer, frontPtr, frontSize);

    memmove(frontPtr, middlePtr, (char*)endPtr - (char*)middlePtr);

    memcpy((char*)endPtr - frontSize, buffer, frontSize);
}

void stack_promote(const Stack* s, const size_t elemIndex, const size_t elemCount)
{
    rotate(get_elem_ptr(s, elemIndex), get_elem_ptr(s, elemIndex + elemCount), get_elem_ptr_simplified(s));
}

void stack_promote_first(const Stack* s, const predicate predicate)
{
    for (size_t i = 0; i < s->currentDepth; i++)
    {
        const void* currentElemPtr = get_elem_ptr(s, i);
        if (predicate(currentElemPtr))
        {
            stack_promote(s, i, 1);
            return;
        }
    }
}

static void swap(void* value1Ptr, void* value2Ptr, const size_t elemSize)
{
    char buffer[elemSize];
    memcpy(buffer, value1Ptr, elemSize);
    memcpy(value1Ptr, value2Ptr, elemSize);
    memcpy(value2Ptr, buffer, elemSize);
}

static size_t partition(const Stack* stackPtr, const size_t startIndex, const size_t endIndex, const get_value_ptr getValuePtr, const bool shouldFree, const is_smaller_than isSmallerThan)
{
    const size_t pivotIndex = endIndex;
    size_t i = startIndex;

    char* pivotPtr = get_elem_ptr(stackPtr, pivotIndex);
    void* pivotValuePtr = getValuePtr(pivotPtr);

    const size_t elemSize = stackPtr->elemSize;

    for (size_t j = startIndex; j <= endIndex - 1; j++)
    {
        char* jPtr = get_elem_ptr(stackPtr, j);
        void* currentValuePtr = getValuePtr(jPtr);
        if (isSmallerThan(currentValuePtr, pivotValuePtr))
        {
            swap(get_elem_ptr(stackPtr, i), jPtr, elemSize);
            i++;
        }

        if (shouldFree)
        {
            free(currentValuePtr);
        }
    }

    if (shouldFree)
    {
        free(pivotValuePtr);
    }

    swap(get_elem_ptr(stackPtr, i), pivotPtr, elemSize);

    return i;
}

static void quick_sort(const Stack* stackPtr, const size_t startIndex, const size_t endIndex, const get_value_ptr getValuePtr, const bool shouldFree, const is_smaller_than isSmallerThan)
{
    if (endIndex <= startIndex)
    {
        return;
    }

    const size_t pivotIndex = partition(stackPtr, startIndex, endIndex, getValuePtr, shouldFree, isSmallerThan);

    if (pivotIndex > startIndex)
    {
        quick_sort(stackPtr, startIndex, pivotIndex - 1, getValuePtr, shouldFree, isSmallerThan);
    }

    if (pivotIndex < endIndex)
    {
        quick_sort(stackPtr, pivotIndex + 1, endIndex, getValuePtr, shouldFree, isSmallerThan);
    }
}

void stack_quick_sort(const Stack* s, const get_value_ptr getValuePtr, const bool shouldFree, const is_smaller_than isSmallerThan)
{
    if (s->currentDepth <= 1)
    {
        return;
    }

    quick_sort(s, 0, s->currentDepth - 1, getValuePtr, shouldFree, isSmallerThan);
}
