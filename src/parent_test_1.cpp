#include <test_cpp/parent_test_1.hpp>
#include <algorithm>


ParentTest1::ParentTest1(TestClass1 member) : member_(std::move(member))
{
}

void ParentTest1::sayHello() const
{
    member_.sayHello();
}
