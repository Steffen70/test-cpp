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

typedef void (*free_elem)(void*);
typedef char* (*to_string)(const void* elemPtr);
typedef char* (*to_string_extended)(void* elemPtr, void (*freeElem)(void*));
typedef bool (*predicate)(const void*);
typedef const void* (*get_value_ptr)(const void* elemPtr);
typedef bool (*is_smaller_than)(const void* jValuePtr, const void* pivotValuePtr);

void stack_init(Stack* s, size_t elemSize, free_elem freeElem);
void stack_destroy(const Stack* s);
void stack_push(Stack* s, const void* valuePtr);
bool stack_pop(Stack* s, void* bufferPtr);
void stack_print(Stack* s, to_string toString, bool shouldFree);
void stack_print_extended(Stack* s, to_string_extended toString, bool shouldFree);
void stack_promote(const Stack* s, size_t elemIndex, size_t elemCount);
void stack_promote_first(const Stack* s, predicate predicate);
void stack_quick_sort(const Stack* s, get_value_ptr getValuePtr, is_smaller_than isSmallerThan);
