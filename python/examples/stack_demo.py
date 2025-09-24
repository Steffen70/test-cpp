import ctypes
import sys
import os

# Add parent directory to path so we can import stacklib
sys.path.insert(0, os.path.dirname(os.path.dirname(__file__)))

from stacklib import Stack


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


print("Sorted integer stack:")
with Stack(elem_size=ctypes.sizeof(ctypes.c_int)) as s:
    for i in reversed(range(1, 8)):
        s.push(ctypes.c_int(i))

    s.sort(int_get_value, int_is_smaller, should_free=False)

    s.print(int_to_string)

print("Unsorted integer stack:")
with Stack(elem_size=ctypes.sizeof(ctypes.c_int)) as s:
    for i in reversed(range(1, 8)):
        s.push(ctypes.c_int(i))

    s.print(int_to_string)


# Demo: push doubles
def double_to_string(elem_ptr):
    val = ctypes.cast(elem_ptr, ctypes.POINTER(ctypes.c_double))[0]
    return f"{val:.2f}"


print("Double stack:")
with Stack(elem_size=ctypes.sizeof(ctypes.c_double)) as s:
    s.push(ctypes.c_double(3.14))
    s.push(ctypes.c_double(2.71))

    s.print(double_to_string)
