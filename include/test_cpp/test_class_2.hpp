#pragma once
#include <string>

class TestClass2
{
public:
    explicit TestClass2(const char* name);
    // No destructor, copy/move construction/assignment needed at all!

    void setName(const char* name);
    void sayHello() const;

private:
    // value-type, handles copy/move automatically
    std::string name_;
};
