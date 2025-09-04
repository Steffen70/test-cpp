#include <test_cpp/stack.hpp>

int main(int argc, char** argv)
{
    Stack myStack;

    myStack.push(1);
    myStack.push(2);
    myStack.push(3);
    myStack.push(4);
    myStack.push(5);
    myStack.push(6);

    myStack.printStack();
    return 0;
}
