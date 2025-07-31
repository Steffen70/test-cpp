#include <test_cpp/test_class_1.hpp>
#include <iostream>
#include <utility>
#include <fmt/core.h>

TestClass1::TestClass1(const char* namePtr) : namePtr_(nullptr)
{
    setName(namePtr);
    std::cout << "TestClass1 created" << '\n';
}

TestClass1 TestClass1::createFromName(const char* namePtr)
{
    return TestClass1(namePtr);
}

TestClass1::~TestClass1() noexcept
{
    delete[] namePtr_;
    std::cout << "TestClass1 destroyed" << '\n';
}

TestClass1::TestClass1(const TestClass1& other) : TestClass1(other.namePtr_)
{
    std::cout << "TestClass1 copied" << '\n';
}

TestClass1& TestClass1::operator=(TestClass1 other)
{
    std::swap(this->namePtr_, other.namePtr_);
    std::cout << "TestClass1 assigned" << '\n';
    return *this;
}

void TestClass1::moveFrom(TestClass1& other) noexcept {
    // Free existing resource
    delete[] namePtr_;
    // Steal and nullify in one line
    namePtr_ = std::exchange(other.namePtr_, nullptr);
}

TestClass1::TestClass1(TestClass1&& other) noexcept : namePtr_(nullptr)
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

void TestClass1::setName(const char* namePtr)
{
    // Freeing a nullptr is safe and does nothing
    delete[] namePtr_;
    if (namePtr)
    {
        namePtr_ = new char[std::strlen(namePtr) + 1];
        std::strcpy(namePtr_, namePtr);
    }
    else
    {
        namePtr_ = new char[1];
        namePtr_[0] = '\0';
    }
}

void TestClass1::sayHello() const
{
    std::cout << fmt::format("Hello, {}!", namePtr_ ? namePtr_ : "(null)") << '\n';
}
