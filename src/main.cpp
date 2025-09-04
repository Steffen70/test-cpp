#include <fmt/color.h>

struct Substitute
{
    int jerseyNumber;
    char* namePtr;
};

struct Player
{
    int number;
    char* namePtr;
};

void swap(void* val1Ptr, void* val2Ptr, const size_t typeSize)
{
    // char buffer[typeSize];
    void* buffer = std::malloc(typeSize);
    memcpy(buffer, val1Ptr, typeSize);
    memcpy(val1Ptr, val2Ptr, typeSize);
    memcpy(val2Ptr, buffer, typeSize);
    std::free(buffer);
}

int main(int argc, char** argv)
{
    fmt::println(stdout, "Size of int64 {}.", sizeof(long));
    fmt::println(stdout, "Size of double {}.", sizeof(double));

    long i(64);
    double d(24);

    fmt::println(stdout, "i: {}, d: {}.", i, d);
    swap(&i, &d, sizeof(double));

    // fmt::println(stdout, "i: {}, d: {}", *reinterpret_cast<double*>(&i), *reinterpret_cast<long*>(&d));
    fmt::println(stdout, "i: {}, d: {}.", std::bit_cast<double>(i), std::bit_cast<long>(d));

    Substitute s(22, strdup("Substitute"));
    Player p(7, strdup("Player"));

    static_assert(std::is_trivially_copyable_v<Substitute> && std::is_trivially_copyable_v<Player>, "must be trivially copyable.");
    static_assert(sizeof(Substitute) == sizeof(Player), "sizes must match.");
    static_assert(alignof(Substitute) == alignof(Player), "alignments must match.");

    fmt::println(stdout, "Substitute: {} #{}, Player: {}, #{}.", s.namePtr, s.jerseyNumber, p.namePtr, p.number);

    swap(&s, &p, sizeof(Player));

    fmt::println(stdout, "Substitute: {} #{}, Player: {}, #{}.", s.namePtr, s.jerseyNumber, p.namePtr, p.number);

    std::free(s.namePtr);
    std::free(p.namePtr);

    int i32(SHRT_MAX * 3 + 2);
    short i16Arr[2];
    i16Arr[0] = 16;

    fmt::println(stdout, "i32: {}, i16: {}.", i32, i16Arr[0]);
    fmt::println(stdout, "i32 as short array, [0]: {}, [1]: {}", ((short*)&i32)[0], ((short*)&i32)[1]);
    swap(&i32, &i16Arr, sizeof(int));

    fmt::println(stdout, "i32: {}, i16[0]: {}, i16[1]: {}, i16Arr as int32: {}.", i32, i16Arr[0], i16Arr[1], *(int*)&i16Arr);

    return 0;
}
