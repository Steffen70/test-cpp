#pragma once
#include <cstddef>

template <std::size_t TLength>
class TestClass3
{
public:
    // Up to TLength - 1 chars, +1 for null terminator
    static constexpr std::size_t max_name_length = TLength - 1;

    explicit TestClass3(const char* namePtr);
    ~TestClass3() noexcept;

    TestClass3(const TestClass3&) = default;
    TestClass3& operator=(const TestClass3&) = default;

    TestClass3(TestClass3&&) = delete;
    TestClass3& operator=(TestClass3&&) = delete;

    void setName(const char* namePtr);

    void sayHello() const;

protected:
    char name_[TLength];
};

# include <test_cpp/test_class_3.tpp>
