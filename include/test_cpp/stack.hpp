#pragma once

struct Stack
{
    int maxDepth;
    int currentDepth;
    int* stackArrPtr;
    Stack();
    ~Stack();
    void push(int value);
    void pop();
    void printStack();
};
