import ctypes
import sys
import os
import random
from generated import student_pb2

# Add parent directory to path so we can import stacklib
sys.path.insert(0, os.path.dirname(os.path.dirname(__file__)))

from stacklib import Stack

def create_random_subject(subject_id: int) -> student_pb2.Subject:
    subj = student_pb2.Subject()
    subj.subject_name = f"Subject{subject_id}"
    subj.grade = 1.0 + random.random() * 5.0
    return subj


random.seed()
student_list = []

for i in range(50):
    stud = student_pb2.Student()
    stud.name = f"Student{i}"
    stud.main_subject.CopyFrom(create_random_subject(i + 1))

    for s in range(4):
        subj = create_random_subject((i + 2) * (s + 1))
        stud.other_subjects.append(subj)

    student_list.append(stud)

print("First:", student_list[0])
print("Last: ", student_list[-1])

# Demo: push integers
def int_to_string(elem_ptr):
    # Cast the void pointer to int pointer and dereference
    val = ctypes.cast(elem_ptr, ctypes.POINTER(ctypes.c_int))[0]
    return str(val)


def int_get_value(elem_ptr):
    # For integers, return the element pointer directly (no extraction needed)
    return elem_ptr


def int_is_smaller(val1_ptr, val2_ptr):
    # Compare two integer pointers
    val1 = ctypes.cast(val1_ptr, ctypes.POINTER(ctypes.c_int))[0]
    val2 = ctypes.cast(val2_ptr, ctypes.POINTER(ctypes.c_int))[0]
    return val1 < val2


with Stack(elem_size=ctypes.sizeof(ctypes.c_int)) as s:
    for i in reversed(range(1, 8)):
        s.push(ctypes.c_int(i))

    s.sort(int_get_value, int_is_smaller, should_free=False)

    s.print(int_to_string)
