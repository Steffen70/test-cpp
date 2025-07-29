#pragma once

class TestClass1
{
public:
    explicit TestClass1(const char* name);
    ~TestClass1() noexcept;

    TestClass1(const TestClass1&);
    TestClass1& operator=(const TestClass1&);

    TestClass1(TestClass1&&) noexcept;
    TestClass1& operator=(TestClass1&&) noexcept;

    void setName(const char* name);

    void sayHello() const;
protected:
    char* name_;
};
