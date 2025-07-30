#include <test_cpp/parent_test_1.hpp>
#include <algorithm>


ParentTest1::ParentTest1(const TestClass1& member) : member_(member)
{
}

// Although 'member' is an rvalue reference parameter, it is a named lvalue with its own identity in this scope, so std::move is needed to cast it back to an rvalue.
ParentTest1::ParentTest1(TestClass1&& member) noexcept : member_(std::move(member))
{
}

void ParentTest1::sayHello() const
{
    member_.sayHello();
}
