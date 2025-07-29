#pragma once

class TestClass
{
public:
    explicit TestClass(const char* name);
    ~TestClass() noexcept;

    TestClass(const TestClass&);
    TestClass& operator=(const TestClass&);

    TestClass(TestClass&&) noexcept;
    TestClass& operator=(TestClass&&) noexcept;

    void setName(const char* name);

    void sayHello() const;
protected:
    char* name_;
};
