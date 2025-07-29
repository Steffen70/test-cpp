#include <test_cpp/test_class.hpp>
#include <utility>

int main(int argc, char** argv)
{
    TestClass test_class1("World");
    TestClass test_class2 = std::move(test_class1);

    TestClass test_class3(test_class2);
    test_class2.setName("Universe");

    test_class1.sayHello();
    test_class2.sayHello();
    test_class3.sayHello();
    return 0;
}
