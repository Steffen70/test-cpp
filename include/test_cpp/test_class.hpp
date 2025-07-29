#pragma once

class TestClass
{
public:
    TestClass(const char* name);
    ~TestClass();

    TestClass(const TestClass&);
    TestClass& operator=(const TestClass&);

    TestClass(TestClass&&);
    TestClass& operator=(TestClass&&);

    void setName(const char* name);

    void sayHello() const;
protected:
    char* name_;
};
