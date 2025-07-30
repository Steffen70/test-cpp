#include <test_cpp/test_class_1.hpp>
#include <iostream>
#include <fmt/core.h>

TestClass1::TestClass1(const char* name)
{
    if (name)
    {
        name_ = new char[std::strlen(name) + 1];
        std::strcpy(name_, name);
    }
    else
    {
        name_ = new char[1];
        name_[0] = '\0';
    }

    std::cout << "TestClass1 created" << '\n';
}

TestClass1 TestClass1::createFromName(const char* name)
{
    return TestClass1(name);
}

TestClass1::~TestClass1() noexcept
{
    delete[] name_;
    std::cout << "TestClass1 destroyed" << '\n';
}

TestClass1::TestClass1(const TestClass1& other) : TestClass1(other.name_)
{
    std::cout << "TestClass1 copied" << '\n';
}

TestClass1& TestClass1::operator=(TestClass1 other)
{
    if (this == &other)
        return *this;

    std::swap(this->name_, other.name_);
    std::cout << "TestClass1 assigned" << '\n';
    return *this;
}

TestClass1::TestClass1(TestClass1&& other) noexcept
{
    this->name_ = other.name_;
    other.name_ = nullptr;
    std::cout << "TestClass1 moved" << '\n';
}

TestClass1& TestClass1::operator=(TestClass1&& other) noexcept
{
    if (this == &other)
        return *this;

    delete[] name_;
    name_ = other.name_;
    other.name_ = nullptr;
    std::cout << "TestClass1 moved assigned" << '\n';
    return *this;
}

void TestClass1::setName(const char* name)
{
    delete[] name_;
    name_ = new char[std::strlen(name) + 1];
    std::strcpy(name_, name);
}

void TestClass1::sayHello() const
{
    std::cout << fmt::format("Hello, {}!", name_ ? name_ : "(null)") << '\n';
}
