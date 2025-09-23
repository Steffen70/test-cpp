import ctypes

from ctypes import c_size_t, c_void_p, c_int, POINTER, Structure, byref


class Stack(Structure):
    _fields_ = [
        ("elemSize", c_size_t),
        ("maxDepth", c_size_t),
        ("currentDepth", c_size_t),
        ("stackArrPtr", c_void_p),
        ("freeElem", c_void_p),
    ]


lib = ctypes.CDLL("../build-debug/stack/libstack.so")

lib.stack_init.argtypes = [POINTER(Stack), c_size_t, c_void_p]
lib.stack_init.restype = None

lib.stack_push.argtypes = [POINTER(Stack), c_void_p]
lib.stack_push.restype = None

lib.stack_print_int.argtypes = [POINTER(Stack)]
lib.stack_print_int.restype = None

lib.stack_destroy.argtypes = [POINTER(Stack)]
lib.stack_destroy.restype = None

s = Stack()

lib.stack_init(byref(s), c_size_t(ctypes.sizeof(c_int)), None)

for i in range(1, 7):
    ci = c_int(i)
    lib.stack_push(byref(s), byref(ci))

lib.stack_print_int(byref(s))

lib.stack_destroy(byref(s))
