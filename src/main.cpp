#include <test_cpp/test_class_1.hpp>
#include <test_cpp/test_class_2.hpp>
#include <test_cpp/test_class_3.hpp>
#include <utility>

void test();

int main(int argc, char** argv)
{
    test();
    return 0;
}

void test()
{
    // Create TestClass1 instance via factory method (on stack)
    auto test_class1 = TestClass1::createFromName("World");
    // Move stack instance to heap-allocated TestClass1; pointer ownership transferred
    auto test_class2 = new TestClass1(std::move(test_class1));

    // Copy-construct test_class3 from heap object test_class2
    TestClass1 test_class3(*test_class2);
    // Change name of test_class2 to verify independent name storage
    test_class2->setName("Universe");

    test_class1.sayHello();
    test_class2->sayHello();
    test_class3.sayHello();

    // TestClass2 follows rule of zero; default copy/move constructors and assignment operators
    auto test_class4 = TestClass2("Switzerland");
    // Move construct test_class5 from test_class4 (uses default move constructor)
    auto test_class5 = std::move(test_class4);

    test_class4.sayHello();
    test_class5.sayHello();

    // TestClass3 stores name in internal fixed array (no heap); copy supported, move not supported
    auto test_class6 = TestClass3("Europe");
    // Default-construct test_class7, then copy-assign from test_class6
    TestClass3 test_class7(nullptr);
    test_class7 = test_class6;
    // Modify test_class6’s name to verify independent storage
    test_class6.setName("Solar System");
    // Default construct test_class8 with nullptr; name set to empty string
    TestClass3 test_class8(nullptr);

    test_class6.sayHello();
    test_class7.sayHello();
    test_class8.sayHello();

    // Create heap instance test_class9 by moving a factory-created stack object
    auto* test_class9 = new TestClass1(std::move(TestClass1::createFromName(nullptr)));
    // Copy-assign the object pointed by test_class2 to the object pointed by test_class9
    *test_class9 = *test_class2;

    test_class2->sayHello();
    test_class9->sayHello();

    // Free heap-allocated objects to avoid memory leaks
    delete test_class2;
    delete test_class9;
}