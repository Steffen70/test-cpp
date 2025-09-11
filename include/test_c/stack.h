#pragma once
#include <stddef.h>

typedef struct Stack
{
    size_t elemSize;
    size_t maxDepth;
    size_t currentDepth;
    void* stackArrPtr;
    void (*freeElem)(void*);
} Stack;

void stack_init(Stack* s, size_t elemSize, void (*freeElem)(void*));
void stack_destroy(const Stack* s);
void stack_push(Stack* s, const void* valuePtr);
bool stack_pop(Stack* s, void* bufferPtr);
void stack_print(Stack* s, char* (*toString)(void* elemPtr), bool shouldFree);
void stack_print_extended(Stack* s, char* (*toString)(void* elemPtr, void (*freeElem)(void*)), bool shouldFree);
void stack_promote(const Stack* s, size_t elemIndex, size_t elemCount);
void stack_promote_first(const Stack* s, bool (*predicate)(void*));
void stack_quick_sort(const Stack* s, void* (*getValuePtr)(void* elemPtr), bool (*isSmallerThan)(const void* jValuePtr, const void* pivotValuePtr));
