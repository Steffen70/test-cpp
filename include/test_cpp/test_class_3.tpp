#pragma once
#include <iostream>
#include <fmt/core.h>

template <std::size_t TLength>
void validate_string_length(const char* namePtr)
{
    typedef TestClass3<TLength> TestClass3;
    if (std::strlen(namePtr) > TestClass3::max_name_length)
        throw std::runtime_error(fmt::format("The provided name can't exceed {} characters.", TestClass3::max_name_length));
}

template <std::size_t TLength>
// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
TestClass3<TLength>::TestClass3(const char* namePtr)
{
    if (!namePtr)
    {
        std::cout << "TestClass3 created" << '\n';
        name_[0] = '\0';
        return;
    }

    validate_string_length<TLength>(namePtr);
    std::cout << "TestClass3 created" << '\n';

    std::strncpy(name_, namePtr, max_name_length);
    // Always terminate.
    name_[max_name_length] = '\0';
}

template <std::size_t TLength>
TestClass3<TLength>::~TestClass3() noexcept
{
    std::cout << "TestClass3 destroyed" << '\n';
}

template <std::size_t TLength>
void TestClass3<TLength>::setName(const char* namePtr)
{
    validate_string_length<TLength>(namePtr);
    std::strncpy(name_, namePtr, max_name_length);
    name_[max_name_length] = '\0';
}

template <std::size_t TLength>
void TestClass3<TLength>::sayHello() const
{
    std::cout << fmt::format("Hello from {}!", name_[0] != '\0' ? name_ : "(null)") << '\n';
}
