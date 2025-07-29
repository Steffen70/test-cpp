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
    TestClass1 test_class1("World");
    auto test_class2 = new TestClass1(std::move(test_class1));

    TestClass1 test_class3(*test_class2);
    test_class2->setName("Universe");

    test_class1.sayHello();
    test_class2->sayHello();
    test_class3.sayHello();

    auto test_class4 = TestClass2("Switzerland");
    auto test_class5 = std::move(test_class4);

    test_class4.sayHello();
    test_class5.sayHello();

    auto test_class6 = TestClass3("Europe");
    auto test_class7 = test_class6;
    test_class6.setName("Solar System");
    TestClass3 test_class8(nullptr);

    test_class6.sayHello();
    test_class7.sayHello();
    test_class8.sayHello();
}