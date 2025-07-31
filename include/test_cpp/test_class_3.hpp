#pragma once
#include <cstddef>

class TestClass3
{
public:
    // Up to 31 visible chars
    static constexpr std::size_t max_name_length = 31;

    explicit TestClass3(const char* namePtr);
    ~TestClass3() noexcept;

    TestClass3(const TestClass3&) = default;
    TestClass3& operator=(const TestClass3&) = default;

    TestClass3(TestClass3&&) = delete;
    TestClass3& operator=(TestClass3&&) = delete;

    void setName(const char* namePtr);

    void sayHello() const;

protected:
    // +1 for null terminator
    char name_[max_name_length + 1];
};
