#include <fmt/color.h>

void* linear_search(void* keyPtr, void* arrPtr, size_t arraySize, size_t elemSize, int (*cmpFn)(void*, void*))
{
    for (size_t i = 0; i < arraySize; i++)
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
    char* namePtr;
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
        Student(4, strdup("Sam")),
        Student(4.5, strdup("Dionys")),
        Student(4.75, strdup("Severin")),
        Student(3.5, strdup("Fredy")),
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

    for (auto& i : studentArr)
    {
        std::free(i.namePtr);
    }
    return 0;
}
