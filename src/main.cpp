#include <test_cpp/test_class_1.hpp>
#include <test_cpp/test_class_2.hpp>
#include <test_cpp/test_class_3.hpp>
#include <test_cpp/parent_test_1.hpp>
#include <iostream>
#include <utility>
#include <fmt/format.h>

struct TestReturnValue
{
    ParentTest1* heapParents;
    ParentTest1** parentPtrs;
    void(ParentTest1::* sayHelloPtr)() const;
};

TestReturnValue test();

int main(int argc, char** argv)
{
    TestReturnValue (* testPtr)() = &test;
    const auto [heapParents, parentPtrs, sayHelloPtr] = (*testPtr)();

    // Confirm updated pointer values and sayHello from heap-constructed objects
    for (std::size_t i = 0; parentPtrs[i] != nullptr; ++i)
    {
        const auto parentPtr = parentPtrs[i];
        std::cout << fmt::format("Heap parentPtr points to {}\n", fmt::ptr(parentPtr));
        (parentPtr->*sayHelloPtr)();
    }

    // delete[] heapParents;

    // Manually destroy each ParentTest1 instance (since we used placement new)
    for (std::size_t i = 0; parentPtrs[i] != nullptr; ++i)
        heapParents[i].~ParentTest1();

    // Free pointer list allocated with new[]
    delete[] parentPtrs;

    // Release the raw memory allocated with malloc
    std::free(heapParents);

    return 0;
}

TestReturnValue test()
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

    // Create ParentTest1 instance parent1 by copying *test2Ptr into its member (copy semantics)
    ParentTest1 parent1(*test2Ptr);
    test2Ptr->sayHello();
    // Delete heap-allocated object test2Ptr after parent1 has made its copy
    delete test2Ptr;
    // Create ParentTest1 instance parent2 by moving *test9Ptr into its member (move semantics)
    ParentTest1 parent2(std::move(*test9Ptr));
    test9Ptr->sayHello();
    test9Ptr->setName("Bern");
    test9Ptr->sayHello();
    // Delete heap-allocated object test9Ptr after stealing the members from it
    delete test9Ptr;

    // parent1 and parent2 have their own independent lifetimes and resources
    // parent1.sayHello();
    void (ParentTest1::* sayHelloPtr)() const = &ParentTest1::sayHello;
    // (parent2.*sayHelloPtr)();

    // Create pointer array to stack-allocated ParentTest1 instances
    ParentTest1* parentPtrs1[] = { &parent1, &parent2 };
    for (const ParentTest1* parentPtr : parentPtrs1)
        // Calls sayHello via pointer-to-member
        (parentPtr->*sayHelloPtr)();

    // Create heap-allocated array of ParentTest1 pointers (ending with nullptr sentinel)
    auto** parentPtrs2 = new  ParentTest1*[]{&parent1, &parent2, nullptr};

    // Iterate over stack-based instances using heap-allocated pointer list
    for (std::size_t i = 0; parentPtrs2[i] != nullptr; ++i)
    {
        const auto parentPtr = parentPtrs2[i];
        std::cout << fmt::format("Stack parentPtr points to {}\n", fmt::ptr(parentPtr));
        (parentPtr->*sayHelloPtr)();
    }

    // Count valid (non-null) pointers
    std::size_t parentCount = 0;
    while (parentPtrs2[parentCount] != nullptr) ++parentCount;

    // Allocate raw memory for heapParents (no construction yet)
    auto heapParents = static_cast<ParentTest1*>(std::malloc(sizeof(ParentTest1) * parentCount));

    // Move-construct ParentTest1 instances into raw memory using placement new
    for (std::size_t i = 0; parentPtrs2[i] != nullptr; ++i)
    {
        const auto parentPtr = parentPtrs2[i];
        new (&heapParents[i]) ParentTest1(std::move(*parentPtr));
        parentPtrs2[i] = &heapParents[i];
    }

    return {
        .heapParents = heapParents,
        .parentPtrs = parentPtrs2,
        .sayHelloPtr = sayHelloPtr
    };
}
