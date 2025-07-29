#include <test_cpp/test_class.hpp>
#include <iostream>
#include <fmt/core.h>
#include <string>

std::string TestClass::sayHello(const std::string& name) const
{
    auto greeting = fmt::format("Hello, {}!", name);
    std::cout << greeting << '\n';
    return greeting;
}
