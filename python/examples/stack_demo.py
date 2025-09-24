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


with Stack(elem_size=ctypes.sizeof(ctypes.c_int)) as s:
    for i in range(1, 8):
        s.push(ctypes.c_int(i))

    s.print(int_to_string)


# Demo: push doubles
def double_to_string(elem_ptr):
    val = ctypes.cast(elem_ptr, ctypes.POINTER(ctypes.c_double))[0]
    return f"{val:.2f}"


with Stack(elem_size=ctypes.sizeof(ctypes.c_double)) as s:
    s.push(ctypes.c_double(3.14))
    s.push(ctypes.c_double(2.71))

    s.print(double_to_string)
