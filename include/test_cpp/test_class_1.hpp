#pragma once

class TestClass1
{
public:
    static TestClass1 createFromName(const char* namePtr);
    ~TestClass1() noexcept;

    TestClass1(const TestClass1&);
    TestClass1& operator=(TestClass1);

    TestClass1(TestClass1&&) noexcept;
    TestClass1& operator=(TestClass1&&) noexcept;

    void setName(const char* namePtr);

    void sayHello() const;

protected:
    explicit TestClass1(const char* namePtr);

    void moveFrom(TestClass1& other) noexcept;

private:
    char* namePtr_;
};
