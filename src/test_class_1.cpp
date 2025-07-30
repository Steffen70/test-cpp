#include <test_cpp/test_class_1.hpp>
#include <iostream>
#include <fmt/core.h>

TestClass1::TestClass1(const char* name)
{
    setName(name);
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
    std::swap(this->name_, other.name_);
    std::cout << "TestClass1 assigned" << '\n';
    return *this;
}

void TestClass1::moveFrom(TestClass1& other) noexcept {
    // free existing resource
    delete[] name_;
    // steal pointer
    name_ = other.name_;
    // nullify moved-from object
    other.name_ = nullptr;
}

TestClass1::TestClass1(TestClass1&& other) noexcept : name_(nullptr)
{
    moveFrom(other);
    std::cout << "TestClass1 moved" << '\n';
}

TestClass1& TestClass1::operator=(TestClass1&& other) noexcept
{
    if (this == &other)
        return *this;

    moveFrom(other);
    std::cout << "TestClass1 moved assigned" << '\n';
    return *this;
}

void TestClass1::setName(const char* name)
{
    // Freeing a nullptr is safe and does nothing.
    delete[] name_;
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
}

void TestClass1::sayHello() const
{
    std::cout << fmt::format("Hello, {}!", name_ ? name_ : "(null)") << '\n';
}
