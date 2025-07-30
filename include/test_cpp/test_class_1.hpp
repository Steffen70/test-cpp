#pragma once

class TestClass1
{
public:
    static TestClass1 createFromName(const char* name);
    ~TestClass1() noexcept;

    TestClass1(const TestClass1&);
    TestClass1& operator=(TestClass1);

    TestClass1(TestClass1&&) noexcept;
    TestClass1& operator=(TestClass1&&) noexcept;

    void setName(const char* name);

    void sayHello() const;

protected:
    explicit TestClass1(const char* name);

    char* name_;
};
