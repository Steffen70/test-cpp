import os
import ctypes
from ctypes import Structure, POINTER, c_size_t, c_void_p, c_bool, c_char_p, CFUNCTYPE

# Mirror struct Stack from stack.h
class _CStack(Structure):
    _fields_ = [
        ("elemSize", c_size_t),
        ("maxDepth", c_size_t),
        ("currentDepth", c_size_t),
        ("stackArrPtr", c_void_p),
        ("freeElem", c_void_p),
    ]

# Load the shared library
_lib_path = os.path.join(os.path.dirname(__file__), "..", "..", "build-debug", "stack", "libstack.so")
_lib = ctypes.CDLL(_lib_path)

# Function prototypes
_lib.stack_init.argtypes = [POINTER(_CStack), c_size_t, c_void_p]
_lib.stack_init.restype = None

_lib.stack_push.argtypes = [POINTER(_CStack), c_void_p]
_lib.stack_push.restype = None

# to_string: char* (*)(const void*)
_to_string_t = CFUNCTYPE(c_char_p, c_void_p)
_lib.stack_print.argtypes = [POINTER(_CStack), _to_string_t, c_bool]
_lib.stack_print.restype = None

_lib.stack_destroy.argtypes = [POINTER(_CStack)]
_lib.stack_destroy.restype = None

# libc for strdup
_libc = ctypes.CDLL("libc.so.6")
_libc.strdup.argtypes = [c_char_p]
_libc.strdup.restype = c_void_p

# Export what the public API needs
__all__ = ['_CStack', '_lib', '_libc', '_to_string_t']
