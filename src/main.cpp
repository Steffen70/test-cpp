#include <test_cpp/test_class_1.hpp>
#include <test_cpp/test_class_2.hpp>
#include <utility>

int main(int argc, char** argv)
{
    TestClass1 test_class1("World");
    TestClass1 test_class2 = std::move(test_class1);

    TestClass1 test_class3(test_class2);
    test_class2.setName("Universe");

    test_class1.sayHello();
    test_class2.sayHello();
    test_class3.sayHello();

    auto test_class4 = TestClass2("Switzerland");
    auto test_class5 = std::move(test_class4);

    test_class4.sayHello();
    test_class5.sayHello();
    return 0;
}
