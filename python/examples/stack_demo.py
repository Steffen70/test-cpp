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


def get_student(elem_ptr) -> tuple[int, student_pb2.Student]:
    # First 8 bytes = size
    data_size = ctypes.cast(elem_ptr, ctypes.POINTER(ctypes.c_size_t))[0]
    data_ptr = ctypes.cast(elem_ptr + ctypes.sizeof(ctypes.c_size_t), ctypes.POINTER(ctypes.c_ubyte))

    # Copy the bytes
    data_bytes = bytes((data_ptr[j] for j in range(data_size)))

    # Deserialize the student
    student = student_pb2.Student()
    student.ParseFromString(data_bytes)

    # Calculate grade average
    grade_average = student.main_subject.grade
    subj_count = 1 + len(student.other_subjects)

    for subj in student.other_subjects:
        grade_average += subj.grade

    grade_average /= subj_count
    return grade_average, student


def student_to_string(elem_ptr):
    grade_average, student = get_student(elem_ptr)
    # Build subject list string
    subj_strings = [f"{student.main_subject.subject_name} ({student.main_subject.grade:.2f})"]

    for subj in student.other_subjects:
        subj_strings.append(f"{subj.subject_name} ({subj.grade:.2f})")

    subj_combined = ", ".join(subj_strings)

    return f"{student.name} ({grade_average:.2f}): {subj_combined}"


def student_is_smaller(val1_ptr, val2_ptr):
    grade_avg1, _ = get_student(val1_ptr)
    grade_avg2, _ = get_student(val2_ptr)

    return grade_avg1 < grade_avg2


# Create student stack like in main.c
random.seed()

# Reasonable max size for student data
MAX_PROTOBUF_SIZE = 1024
# Calculate element size: size_t for length + max protobuf data
STUDENT_ELEMENT_SIZE = ctypes.sizeof(ctypes.c_size_t) + MAX_PROTOBUF_SIZE

with Stack(elem_size=STUDENT_ELEMENT_SIZE) as student_stack:
    for i in range(50):
        stud = student_pb2.Student()
        stud.name = f"Student{i}"
        stud.main_subject.CopyFrom(create_random_subject(i + 1))

        for s in range(4):
            subj = create_random_subject((i + 2) * (s + 1))
            stud.other_subjects.append(subj)

        # Serialize and create element
        serialized = stud.SerializeToString()

        # Create element: [size_t length][serialized data][padding]
        element_data = bytearray(STUDENT_ELEMENT_SIZE)

        # Write size - get the address of the bytearray data
        size_bytes = ctypes.c_size_t(len(serialized))
        ctypes.memmove(ctypes.addressof((ctypes.c_ubyte * len(element_data)).from_buffer(element_data)),
                       ctypes.addressof(size_bytes),
                       ctypes.sizeof(ctypes.c_size_t))

        # Write serialized data
        ctypes.memmove(ctypes.addressof((ctypes.c_ubyte * len(element_data)).from_buffer(element_data)) + ctypes.sizeof(ctypes.c_size_t),
                       serialized,
                       len(serialized))

        student_stack.push(bytes(element_data))

    student_stack.sort(student_is_smaller)

    student_stack.print(student_to_string)


# Demo: push integers
def int_to_string(elem_ptr):
    # Cast the void pointer to int pointer and dereference
    val = ctypes.cast(elem_ptr, ctypes.POINTER(ctypes.c_int))[0]
    return str(val)


def int_is_smaller(val1_ptr, val2_ptr):
    # Compare two integer pointers
    val1 = ctypes.cast(val1_ptr, ctypes.POINTER(ctypes.c_int))[0]
    val2 = ctypes.cast(val2_ptr, ctypes.POINTER(ctypes.c_int))[0]
    return val1 < val2


with Stack(elem_size=ctypes.sizeof(ctypes.c_int)) as s:
    for i in reversed(range(1, 8)):
        s.push(ctypes.c_int(i))

    s.sort(int_is_smaller)

    s.print(int_to_string)
