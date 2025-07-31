#include <test_cpp/test_class_3.hpp>
#include <iostream>
#include <fmt/core.h>

void validate_string_length(const char* namePtr)
{
    if (std::strlen(namePtr) > TestClass3::max_name_length)
        throw std::runtime_error(fmt::format("The provided name can't exceed {} characters.", TestClass3::max_name_length));
}

// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
TestClass3::TestClass3(const char* namePtr)
{
    if (!namePtr)
    {
        std::cout << "TestClass3 created" << '\n';
        name_[0] = '\0';
        return;
    }

    validate_string_length(namePtr);
    std::cout << "TestClass3 created" << '\n';

    std::strncpy(name_, namePtr, max_name_length);
    // Always terminate.
    name_[max_name_length] = '\0';
}

TestClass3::~TestClass3() noexcept
{
    std::cout << "TestClass3 destroyed" << '\n';
}

// TestClass3::TestClass3(const TestClass3& other)
// {
//     std::strncpy(name_, other.name_, max_name_length);
//     name_[max_name_length] = '\0';
//     std::cout << "TestClass3 copied" << '\n';
// }
//
// TestClass3& TestClass3::operator=(const TestClass3& other)
// {
//     if (this == &other)
//         return *this;
//
//     std::strncpy(name_, other.name_, max_name_length);
//     name_[max_name_length] = '\0';
//
//     std::cout << "TestClass3 assigned" << '\n';
//     return *this;
// }

void TestClass3::setName(const char* namePtr)
{
    validate_string_length(namePtr);
    std::strncpy(name_, namePtr, max_name_length);
    name_[max_name_length] = '\0';
}

void TestClass3::sayHello() const
{
    std::cout << fmt::format("Hello, {}!", name_[0] != '\0' ? name_ : "(null)") << '\n';
}