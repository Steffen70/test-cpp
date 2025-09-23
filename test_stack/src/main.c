#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <student.pb-c.h>
#include <time.h>
#include <stack/stack.h>

static void stud_free_elem(void* elemPtr)
{
    Test__Stack__Student* studPtr = elemPtr;
    if (studPtr->name != nullptr)
    {
        free(studPtr->name);
        studPtr->name = nullptr;
    }

    for (size_t i = 0; i < studPtr->n_other_subjects; i++)
    {
        test__stack__subject__free_unpacked(studPtr->other_subjects[i], nullptr);
        studPtr->other_subjects[i] = nullptr;
    }

    if (studPtr->other_subjects != nullptr)
    {
        free(studPtr->other_subjects);
        studPtr->other_subjects = nullptr;
    }
    studPtr->n_other_subjects = 0;

    if (studPtr->main_subject != nullptr)
    {
        test__stack__subject__free_unpacked(studPtr->main_subject, nullptr);
        studPtr->main_subject = nullptr;
    }
}

static char* string_join(const char* seperatorPtr, char** stringPtrArr, const size_t n)
{
    size_t strLenArr[n];
    const size_t seperatorLen = strlen(seperatorPtr);
    size_t bufferSize = seperatorLen * (n - 1) + 1;
    for (size_t i = 0; i < n; i++)
    {
        strLenArr[i] = strlen(stringPtrArr[i]);
        bufferSize += strLenArr[i];
    }

    char* bufferPtr = malloc(bufferSize);
    auto lastInsertPtr = bufferPtr;
    for (size_t i = 0; i < n; i++)
    {
        memcpy(lastInsertPtr, stringPtrArr[i], strLenArr[i]);
        lastInsertPtr += strLenArr[i];
        if (i < n - 1)
        {
            memcpy(lastInsertPtr, seperatorPtr, seperatorLen);
            lastInsertPtr += seperatorLen;
        }
    }

    *lastInsertPtr = '\0';

    return bufferPtr;
}

static char* stud_to_string_extended(void* elemPtr, const free_elem freeElem)
{
    const Test__Stack__Student* studPtr = elemPtr;

    double gradeAverage = 0;
    const size_t subjCount = studPtr->n_other_subjects + 1;
    char* subjNames[subjCount];
    for (size_t i = 0; i < subjCount; i++)
    {
        char buffer[32];
        double grade = studPtr->main_subject->grade;
        char* namePtr = studPtr->main_subject->subject_name;
        if (i != 0)
        {
            grade = studPtr->other_subjects[i - 1]->grade;
            namePtr = studPtr->other_subjects[i - 1]->subject_name;
        }

        gradeAverage += grade;

        snprintf(buffer, sizeof(buffer), "%s (%.2f)", namePtr, grade);
        subjNames[i] = strdup(buffer);
    }
    char* subjCombined = string_join(", ", subjNames, subjCount);

    for (size_t i = 0; i < subjCount; i++)
    {
        free(subjNames[i]);
    }

    gradeAverage /= (double)subjCount;

    const size_t bufferSize = strlen(subjCombined) + strlen(studPtr->name) + 16;
    char* bufferPtr = malloc(bufferSize);
    snprintf(bufferPtr, bufferSize, "%s (%.2f): %s", studPtr->name, gradeAverage, subjCombined);
    free(subjCombined);

    freeElem(elemPtr);
    return bufferPtr;
}

static Test__Stack__Subject* create_subject(size_t subjNum, double grade)
{
    // ReSharper disable once CppDFAMemoryLeak
    Test__Stack__Subject* subjPtr = malloc(sizeof(Test__Stack__Subject));
    test__stack__subject__init(subjPtr);
    char subjNameBuffer[16];
    snprintf(subjNameBuffer, sizeof(subjNameBuffer), "Subject%zu", subjNum);
    subjPtr->subject_name = strdup(subjNameBuffer);
    subjPtr->grade = grade;
    return subjPtr;
}

static void* stud_get_value_ptr(const void* elemPtr)
{
    const Test__Stack__Student* studPtr = elemPtr;

    double* gradeAveragePtr = malloc(sizeof(double));
    *gradeAveragePtr = 0;
    const size_t subjCount = studPtr->n_other_subjects + 1;
    for (size_t i = 0; i < subjCount; i++)
    {
        double grade = studPtr->main_subject->grade;
        if (i != 0)
        {
            grade = studPtr->other_subjects[i - 1]->grade;
        }
        *gradeAveragePtr += grade;
    }

    *gradeAveragePtr /= (double)subjCount;
    return gradeAveragePtr;
}

static bool double_is_smaller_than(const void* jValuePtr, const void* pivotValuePtr)
{
    return *(double*)jValuePtr < *(double*)pivotValuePtr;
}

int main()
{
    Stack studStack;

    stack_init(&studStack, sizeof(Test__Stack__Student), &stud_free_elem);

    srand48(time(nullptr));
    for (size_t i = 0; i < 3; i++)
    {
        Test__Stack__Student stud = TEST__STACK__STUDENT__INIT;
        char nameBuffer[16];
        snprintf(nameBuffer, sizeof(nameBuffer), "Student%zu", i);
        stud.name = strdup(nameBuffer);
        stud.n_other_subjects = 4;
        stud.other_subjects = malloc(sizeof(Test__Stack__Subject*) * stud.n_other_subjects);
        stud.main_subject = create_subject(i + 1, 1.0 + drand48() * 5.0);

        for (size_t s = 0; s < stud.n_other_subjects; s++)
        {
            // ReSharper disable once CppDFAMemoryLeak
            stud.other_subjects[s] = create_subject(s + 1 * (i + 2), 1.0 + drand48() * 5.0);
        }
        stack_push(&studStack, &stud);
    }

    stack_quick_sort(&studStack, &stud_get_value_ptr, true, &double_is_smaller_than);

    stack_print_extended(&studStack, &stud_to_string_extended, true);

    stack_destroy(&studStack);

    return 0;
}
