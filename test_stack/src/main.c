#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stack/stack.h>

typedef struct Student
{
    double grade;
    char* name;
} Student;

static const void* int_get_value_ptr(const void* elemPtr)
{
    return (void*)elemPtr;
}

static bool int_is_smaller_than(const void* jValuePtr, const void* pivotValuePtr)
{
    return *(int*)jValuePtr < *(int*)pivotValuePtr;
}

static char* int_to_string(const void* elemPtr)
{
    const int* intPtr = elemPtr;
    char numStrBuffer[16];
    snprintf(numStrBuffer, sizeof(numStrBuffer), "%d", *intPtr);
    return strdup(numStrBuffer);
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
static void student_free_elem(void* elemPtr)
{
    const Student* studPtr = elemPtr;
    free(studPtr->name);
}

static bool student_predicate(const void* elemPtr)
{
    return ((Student*)elemPtr)->grade == 6;
}

static const void* student_get_value_ptr(const void* elemPtr)
{
    return &((Student*)elemPtr)->grade;
}

static bool student_is_smaller_than(const void* jValuePtr, const void* pivotValuePtr)
{
    return *(double*)jValuePtr < *(double*)pivotValuePtr;
}

static char* student_to_string(void* elemPtr, const free_elem freeElem)
{
    const Student* studPtr = elemPtr;
    char* messagePtr = malloc(64);
    snprintf(messagePtr, 64, "%s with grade: %f", studPtr->name, studPtr->grade);
    freeElem(elemPtr);
    return messagePtr;
}

static void str_free_elem(void* elemPtr)
{
    free(*(char**)elemPtr);
}

static char* str_to_string(const void* elemPtr)
{
    return *(char**)elemPtr;
}

static const void* str_get_value_ptr(const void* elemPtr)
{
    return *(char**)elemPtr;
}

static bool str_is_smaller_than(const void* jValuePtr, const void* pivotValuePtr)
{
    static regex_t re;
    static bool isRegexInited = false;
    if (!isRegexInited)
    {
        regcomp(&re, "^Friend([0-9]+)$", REG_EXTENDED);
        isRegexInited = true;
    }

    regmatch_t jMatch[2], pivotMatch[2];
    // NOLINTNEXTLINE(*-unused-return-value)
    regexec(&re, jValuePtr, 2, jMatch, 0);
    // NOLINTNEXTLINE(*-unused-return-value)
    regexec(&re, pivotValuePtr, 2, pivotMatch, 0);

    const int jNum = (int)strtol(jValuePtr+jMatch[1].rm_so, nullptr, 10);
    const int pivotNum = (int)strtol(pivotValuePtr+jMatch[1].rm_so, nullptr, 10);

    return jNum < pivotNum;
}

int main(int argc, char** argv)
{
    Stack intStack;
    stack_init(&intStack, sizeof(int), nullptr);

    for (int i = 1; i <= 6; i++)
    {
        stack_push(&intStack, &i);
    }

    stack_promote(&intStack, 1, 3);
    stack_quick_sort(&intStack, &int_get_value_ptr, &int_is_smaller_than);


    stack_print(&intStack, &int_to_string, true);

    stack_destroy(&intStack);

    Stack studStack;
    stack_init(&studStack, sizeof(Student), &student_free_elem);

    constexpr size_t maxCount = 1001;
    constexpr size_t gradeFactor = (maxCount - 1) / 10;
    for (size_t i = 0; i < maxCount; i++)
    {
        Student stud;
        stud.grade = (double)i / gradeFactor / 2 + 1;
        char nameBuffer[32];
        snprintf(nameBuffer, sizeof(nameBuffer), "Student%zu", i);
        stud.name = strdup(nameBuffer);
        stack_push(&studStack, &stud);
    }

    stack_promote(&studStack, 800, 100);
    stack_promote(&studStack, 420, 1);
    stack_promote(&studStack, 69, 1);
    stack_promote_first(&studStack, &student_predicate);

    stack_quick_sort(&studStack, &student_get_value_ptr, &student_is_smaller_than);

    stack_print_extended(&studStack, &student_to_string, true);

    Stack strStack;
    stack_init(&strStack, sizeof(char*), &str_free_elem);

    for (size_t i = 0; i < 3; i++)
    {
        const char* friendsArr[] = {
            "Friend161",
            "Friend24",
            "Friend3"
        };
        char* stringCopyPtr = strdup(friendsArr[i]);
        stack_push(&strStack, &stringCopyPtr);
    }

    stack_promote(&strStack, 1, 1);

    stack_quick_sort(&strStack, &str_get_value_ptr, &str_is_smaller_than);

    stack_print(&strStack, &str_to_string, true);

    return 0;
}
