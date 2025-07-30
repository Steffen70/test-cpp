#include <test_cpp/test_class_1.hpp>
#include <test_cpp/test_class_2.hpp>
#include <test_cpp/test_class_3.hpp>
#include <utility>

#include "test_cpp/parent_test_1.hpp"

void test();

int main(int argc, char** argv)
{
    test();
    return 0;
}

void test()
{
    // Create TestClass1 instance via factory method (on stack)
    auto test1 = TestClass1::createFromName("World");
    // Move stack instance to heap-allocated TestClass1; pointer ownership transferred of all members
    const auto test2Ptr = new TestClass1(std::move(test1));

    // Copy-construct test3 from heap object *test2Ptr
    const TestClass1 test3(*test2Ptr);
    // Change name of *test2Ptr to verify independent name storage
    test2Ptr->setName("Universe");

    test1.sayHello();
    test2Ptr->sayHello();
    test3.sayHello();

    // TestClass2 follows rule of zero; default copy/move constructors and assignment operators
    auto test4 = TestClass2("Switzerland");
    // Move construct test5 from test4 (uses default move constructor)
    const auto test5 = std::move(test4);

    test4.sayHello();
    test5.sayHello();

    // TestClass3 stores name in internal fixed array (no heap); copy supported, move not supported
    auto test6 = TestClass3("Europe");
    // Default-construct test7, then copy-assign from test6
    TestClass3 test7(nullptr);
    test7 = test6;
    // Modify test6’s name to verify independent storage
    test6.setName("Solar System");
    // Default construct test8 with nullptr; name set to empty string
    const TestClass3 test8(nullptr);

    test6.sayHello();
    test7.sayHello();
    test8.sayHello();

    // Create heap instance *test9Ptr by moving a factory-created stack object
    auto* test9Ptr = new TestClass1(std::move(TestClass1::createFromName(nullptr)));
    // Copy-assign the object pointed by test2Ptr to the object pointed by test9Ptr
    *test9Ptr = *test2Ptr;

    test2Ptr->sayHello();
    test9Ptr->sayHello();

    // Free heap-allocated objects to avoid memory leaks
    // delete test2Ptr;
    // delete test9Ptr;

    const ParentTest1 parent1(*test2Ptr);
    delete test2Ptr;
    const ParentTest1 parent2(*test9Ptr);
    delete test9Ptr;

    parent1.sayHello();
    parent2.sayHello();
}