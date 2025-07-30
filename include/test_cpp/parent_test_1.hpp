#pragma once
#include "test_cpp/test_class_1.hpp"

class ParentTest1 {
public:
    // No need to write ctor/dtor/assignment explicitly if only members are well-behaved
    explicit ParentTest1(const TestClass1& member);

    explicit ParentTest1(TestClass1&& member) noexcept;

    void sayHello() const;
private:
    // RAII type handling resource internally
    TestClass1 member_;
};
