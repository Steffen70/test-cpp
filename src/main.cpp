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

// int cmpFn(void* val1Ptr, void* val2Ptr)
// {
//     return memcmp(val1Ptr, val2Ptr, sizeof(int));
// }

int main(int argc, char** argv)
{
    int intArr[] = {10, 20, 30, 40, 50};
    int key = 40;

    // void* elemAddrPtr = linear_search(&key, &intArr, 5, sizeof(int), +cmpFn);
    void* elemAddrPtr = linear_search(&key, &intArr, 5, sizeof(int), +[](void* val1Ptr, void* val2Ptr) -> int {
        return memcmp(val1Ptr, val2Ptr, sizeof(int));
    });

    fmt::println(stdout, "elemAddrPtr: {}, elemValue: {}, key: {}", fmt::ptr(elemAddrPtr), *(int*)elemAddrPtr, key);
    return 0;
}
