#include <test_cpp/test_class_2.hpp>
#include <iostream>
#include <fmt/core.h>

TestClass2::TestClass2(const char* name) : name_(name ? name : "")
{
    std::cout << "TestClass2 created" << '\n';
}

void TestClass2::setName(const char* name)
{
    name_ = name ? name : "";
}

void TestClass2::sayHello() const
{
    std::cout << fmt::format("Hello, {}!", !name_.empty() ? name_ : "(null)") << '\n';
}
