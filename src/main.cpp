#include <fmt/color.h>

void* linear_search(void* keyPtr, void* arrPtr, size_t arrSize, size_t elemSize, int (*cmpFn)(void*, void*))
{
    for (size_t i = 0; i < arrSize; i++)
    {
        void* elemAddrPtr = (char*)arrPtr + i * elemSize;
        if (cmpFn(keyPtr, elemAddrPtr) == 0)
        {
            return elemAddrPtr;
        }
    }

    return nullptr;
}

struct Student
{
    double grade;
    const char* namePtr;
};

int main(int argc, char** argv)
{
    int intArr[] = {10, 20, 30, 40, 50};
    int key = 40;

    // void* elemAddrPtr = linear_search(&key, &intArr, 5, sizeof(int), +cmpFn);
    void* elemAddrPtr = linear_search(&key, &intArr, 5, sizeof(int), +[](void* val1Ptr, void* val2Ptr) -> int
    {
        return memcmp(val1Ptr, val2Ptr, sizeof(int));
    });

    fmt::println(stdout, "elemAddrPtr: {}, elemValue: {}, key: {}", fmt::ptr(elemAddrPtr), *(int*)elemAddrPtr, key);

    Student studentArr[] = {
        Student(4, "Sam"),
        Student(4.5, "Dionys"),
        Student(4.75, "Severin"),
        Student(3.5, "Fredy"),
    };

    for (size_t i = 6 * 4; i >= 1 * 4; i--)
    {
        double grade = static_cast<double>(i) / 4;
        Student* bestStudentPtr = (Student*)linear_search(&grade, &studentArr, 4, sizeof(Student), +[](void* gradePtr, void* studPtr) -> int
        {
            double diff = ((Student*)studPtr)->grade - *(double*)gradePtr;
            if (diff == 0) return 0;
            return diff < 0 ? -1 : 1;
        });

        if (bestStudentPtr == nullptr) continue;

        fmt::println(stdout, "The best student is: {}, with grade: {}", bestStudentPtr->namePtr, bestStudentPtr->grade);
        break;
    }

    const char* strArr[]
    {
        "Test",
        "Test1",
        "Test2",
        "Test3"
    };

    char** matchingStrPtr = (char**)linear_search((void*)"Test1", strArr, 4, sizeof(const char*), [](void* keyPtr, void* elemPtr) -> int
    {
        return strcmp((char*)keyPtr, *(char**)elemPtr);
    });

    if (matchingStrPtr != nullptr)
        fmt::println(stdout, "The matching string is: {}", *matchingStrPtr);

    return 0;
}
