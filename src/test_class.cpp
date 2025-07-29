#include <test_cpp/test_class.hpp>
#include <iostream>
#include <fmt/core.h>

TestClass::TestClass(const char* name)
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

    std::cout << "TestClass created" << '\n';
}

TestClass::~TestClass()
{
    delete[] name_;
    std::cout << "TestClass destroyed" << '\n';
}

TestClass::TestClass(const TestClass& other)
{
    name_ = new char[std::strlen(other.name_) + 1];
    std::strcpy(name_, other.name_);
    std::cout << "TestClass copied" << '\n';
}

TestClass& TestClass::operator=(const TestClass& other)
{
    if (this == &other)
        throw std::runtime_error("Cannot assign to self");

    delete[] name_; // Free old name_
    name_ = new char[std::strlen(other.name_) + 1];
    std::strcpy(name_, other.name_);

    std::cout << "TestClass assigned" << '\n';
    return *this;
}

TestClass::TestClass(TestClass&& other)
{
    this->name_ = other.name_;
    other.name_ = nullptr;
    std::cout << "TestClass moved" << '\n';
}

TestClass& TestClass::operator=(TestClass&& other)
{
    if (this == &other)
        throw std::runtime_error("Cannot assign to self");

    delete[] name_;
    name_ = other.name_;
    other.name_ = nullptr;
    std::cout << "TestClass moved assigned" << '\n';
    return *this;
}

void TestClass::setName(const char* name)
{
    delete[] name_;
    name_ = new char[std::strlen(name) + 1];
    std::strcpy(name_, name);
}

void TestClass::sayHello() const
{
    std::cout << fmt::format("Hello, {}!", name_ ? name_ : "(null)") << '\n';
}
