#include <charconv>
#include <regex>
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

    intStack.promote(1, 3);
    intStack.quickSort(
        [](void* elemPtr) -> void*
        {
            return elemPtr;
        }, [](void* jValuePtr, void* pivotValuePtr) -> bool
        {
            return *(int*)jValuePtr < *(int*)pivotValuePtr;
        });


    intStack.printStack([](void* elemPtr) -> char*
    {
        auto* intPtr = (int*)elemPtr;
        return strdup(std::to_string(*intPtr).c_str());
    }, true);

    Stack studStack(sizeof(Student), [](void* elemPtr) -> void
    {
        auto* studPtr = (Student*)elemPtr;
        std::free(studPtr->name);
    });

    constexpr size_t maxCount = 1001;
    constexpr size_t gradeFactor = (maxCount - 1) / 10;
    for (size_t i = 0; i < maxCount; i++)
    {
        auto* studentPtr = new Student((double)i / gradeFactor / 2 + 1, strdup(fmt::format("Student{}", i).c_str()));
        studStack.push(studentPtr);
        delete studentPtr;
    }

    studStack.promote(800, 100);
    studStack.promote(420);
    studStack.promote(69);
    studStack.promoteFirst([](void* elemPtr) -> bool
    {
        return ((Student*)elemPtr)->grade == 6;
    });

    studStack.quickSort(
        [](void* elemPtr) -> void*
        {
            return &((Student*)elemPtr)->grade;
        }, [](void* jValuePtr, void* pivotValuePtr) -> bool
        {
            return *(double*)jValuePtr < *(double*)pivotValuePtr;
        });

    studStack.printStack([](void* elemPtr, void (*freeElem)(void*)) -> char*
    {
        auto* studPtr = (Student*)elemPtr;
        auto* messagePtr = strdup(fmt::format("{} with grade: {}", studPtr->name, studPtr->grade).c_str());
        freeElem(elemPtr);
        return messagePtr;
    }, true);

    const char* friendsArr[] = {
        "Friend161",
        "Friend24",
        "Friend3"
    };

    Stack strStack(sizeof(char*), [](void* elemPtr) -> void
    {
        std::free(*(char**)elemPtr);
    });

    for (const char* friendPtr : friendsArr)
    {
        auto* stringCopyPtr = strdup(friendPtr);
        strStack.push(&stringCopyPtr);
    }

    strStack.promote(1);

    strStack.quickSort(
        [](void* elemPtr) -> void*
        {
            return *(char**)elemPtr;
        }, [](void* jValuePtr, void* pivotValuePtr) -> bool
        {
            static const std::regex numRegex("^Friend(\\d+)$");

            std::cmatch jMatch, pivotMatch;
            std::regex_search((char*)jValuePtr, jMatch, numRegex);
            std::regex_search((char*)pivotValuePtr, pivotMatch, numRegex);

            int jNum = 0, pivotNum = 0;
            std::from_chars(jMatch[1].first, jMatch[1].second, jNum);
            std::from_chars(pivotMatch[1].first, pivotMatch[1].second, pivotNum);
            return jNum < pivotNum;
        });


    strStack.printStack([](void* friendPtr, void (*freeElem)(void*)) -> char*
    {
        // return *(char**)friendPtr;
        auto* messagePtr = strdup(fmt::format("My friend is: {}", *(char**)friendPtr).c_str());
        freeElem(friendPtr);
        return messagePtr;
    }, true);

    return 0;
}
