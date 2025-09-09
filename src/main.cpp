#include <fmt/core.h>
#include <string>
#include <test_cpp/stack.hpp>

struct Student
{
    double grade;
    char* name;
};

int main(int argc, char** argv)
{
    Stack intStack(sizeof(int));

    for (int i = 1; i <= 6; i++)
    {
        intStack.push(&i);
    }

    intStack.printStack([](void* elemPtr) -> char*
    {
        auto* intPtr = (int*)elemPtr;
        return strdup(std::to_string(*intPtr).c_str());
    }, true);

    Stack studStack(sizeof(Student));

    constexpr size_t maxCount = 1000;
    constexpr size_t gradeFactor = maxCount / 10;
    for (size_t i = 0; i <= maxCount; i++)
    {
        auto* studentPtr = new Student((double)i / gradeFactor / 2 + 1, strdup(fmt::format("Student{}", i).c_str()));
        studStack.push(studentPtr);
    }

    studStack.printStack([](void* elemPtr) -> char*
    {
        auto* studPtr = (Student*)elemPtr;
        return strdup(fmt::format("{} with grade: {}", studPtr->name, studPtr->grade).c_str());
    }, true);

    const char* friendsArr[] = {
        "Friend1",
        "Friend2",
        "Friend3"
    };

    Stack strStack(sizeof(const char*));
    for (const char* friendPtr : friendsArr)
    {
        strStack.push(friendPtr);
    }

    strStack.printStack([](void* friendPtr) -> char*
    {
        return (char*)friendPtr;
    }, false);

    return 0;
}
