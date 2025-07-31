#include <test_cpp/test_class_1.hpp>
#include <iostream>
#include <unordered_map>
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

void TestClass1::moveFrom(TestClass1& other) noexcept
{
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

static constexpr u_int16_t europe = 1000;

int string_to_location(const char* strPtr)
{
    if (!strPtr) return -1;

    // Static map to avoid re-creation every call
    static const std::unordered_map<std::string_view, u_int16_t> lookup{
        {"Bern", 0},
        {"Zürich", 1},
        {"Basel", 1},
        {"Switzerland", 100},
        {"Europe", europe},
        {"NewYork", 1001},
        {"LosAngeles", 1002},
        {"Chicago", 1003},
        {"America", 1100},
        {"USA", 1100},
        {"Tokyo", 1101},
        {"Japan", 1200},
        {"World", 10000},
        {"SolarSystem", 100000},
        {"Universe", 1000000}
    };

    // No allocation, just view
    const std::string_view key = strPtr;
    if (const auto it = lookup.find(key); it != lookup.end())
        return it->second;
    return -1;
}

TestClass1::operator bool() const
{
    // Early return false if namePtr_ is null or points to an empty string
    if (namePtr_ == nullptr || *namePtr_ == '\0')
        return false;

    // Calculate the length of the input string excluding spaces
    const char* strPtr = namePtr_;
    std::size_t len = 0;
    do
    {
        // count only non-space characters
        if (*strPtr != ' ')
            len++;
        strPtr++;
    }
    while (*strPtr != '\0');

    // Create a buffer on stack to hold the input string without spaces
    char nameNoSpace[len];
    const char* nextPtr = namePtr_;
    std::size_t i = 0;
    // Copy non-space characters from the input into the buffer
    for (; i < len; ++i)
    {
        while (*nextPtr == ' ')
            ++nextPtr;
        nameNoSpace[i] = *nextPtr;
        nextPtr++;
    }
    // Null-terminate the cleaned string
    nameNoSpace[i] = '\0';

    // Convert the cleaned string into a location value using the lookup map
    const int locationInt = string_to_location(&nameNoSpace[0]);
    if (locationInt == -1)
        return false;

    // Return true if the mapped location value is less than or equal to 'Europe' threshold
    return locationInt <= static_cast<int>(europe);
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
    std::cout << fmt::format("Hello from {}!", namePtr_ ? namePtr_ : "(null)") << '\n';
}
