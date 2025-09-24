import os
import ctypes
from ctypes import Structure, POINTER, byref, c_size_t, c_void_p, c_bool, c_int, c_char_p, CFUNCTYPE


# Mirror struct Stack from stack.h
class Stack(Structure):
    _fields_ = [
        ("elemSize", c_size_t),
        ("maxDepth", c_size_t),
        ("currentDepth", c_size_t),
        ("stackArrPtr", c_void_p),
        ("freeElem", c_void_p),
    ]


# Load the shared library
lib = ctypes.CDLL(os.path.join(os.path.dirname(__file__), "..", "build-debug", "stack", "libstack.so"))

# Prototypes
lib.stack_init.argtypes = [POINTER(Stack), c_size_t, c_void_p]
lib.stack_init.restype = None

lib.stack_push.argtypes = [POINTER(Stack), c_void_p]
lib.stack_push.restype = None

# to_string: char* (*)(const void*)
to_string_t = CFUNCTYPE(c_char_p, c_void_p)
lib.stack_print.argtypes = [POINTER(Stack), to_string_t, c_bool]
lib.stack_print.restype = None

lib.stack_destroy.argtypes = [POINTER(Stack)]
lib.stack_destroy.restype = None

libc = ctypes.CDLL("libc.so.6")
libc.strdup.argtypes = [ctypes.c_char_p]
libc.strdup.restype = ctypes.c_void_p


# Python to_string for int elements
def py_int_to_string(elem_ptr):
    val = ctypes.cast(elem_ptr, ctypes.POINTER(c_int))[0]
    return libc.strdup(str(val).encode("utf-8"))


# keep a global reference
INT_TO_STRING_CB = to_string_t(py_int_to_string)

# Demo: push 1..7 and print
s = Stack()
lib.stack_init(byref(s), c_size_t(ctypes.sizeof(c_int)), None)

for i in range(1, 8):
    ci = c_int(i)
    lib.stack_push(byref(s), byref(ci))

lib.stack_print(byref(s), INT_TO_STRING_CB, True)

lib.stack_destroy(byref(s))
