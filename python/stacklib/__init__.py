import ctypes
from ._ffi import *


class Stack:

    def __init__(self, elem_size: int):
        self._stack = _CStack()
        self._to_string_callback = None
        self._get_value_callback = None
        self._is_smaller_callback = None
        self._closed = False
        _lib.stack_init(ctypes.byref(self._stack), ctypes.c_size_t(elem_size), None)

    def push(self, value):
        if isinstance(value, (bytes, bytearray)):
            # Raw bytes - create a buffer and copy
            if len(value) != self._stack.elemSize:
                raise ValueError(f"Bytes length {len(value)} doesn't match element size {self._stack.elemSize}")
            buffer = (ctypes.c_ubyte * len(value)).from_buffer_copy(value)
            _lib.stack_push(ctypes.byref(self._stack), ctypes.cast(buffer, ctypes.c_void_p))
        else:
            # Try to get a pointer from the object
            try:
                _lib.stack_push(ctypes.byref(self._stack), ctypes.byref(value))
            except TypeError:
                raise TypeError(f"Cannot push object of type {type(value)}. Use ctypes objects (c_int, c_double, etc.) or raw bytes.")

    def print(self, to_string_fn):
        if not callable(to_string_fn):
            raise TypeError("to_string_fn must be callable")

        def _adapter(elem_ptr):
            # Pass the raw void pointer to user code
            result = to_string_fn(elem_ptr)
            # strdup for C to free
            return _libc.strdup(result.encode("utf-8"))

        self._to_string_callback = _to_string_t(_adapter)
        _lib.stack_print(ctypes.byref(self._stack), self._to_string_callback, True)

    def sort(self, get_value_fn, is_smaller_fn, should_free=False):
        if not callable(get_value_fn):
            raise TypeError("get_value_fn must be callable")
        if not callable(is_smaller_fn):
            raise TypeError("is_smaller_fn must be callable")

        def _get_value_adapter(elem_ptr):
            return get_value_fn(elem_ptr)

        def _is_smaller_adapter(val1_ptr, val2_ptr):
            return is_smaller_fn(val1_ptr, val2_ptr)

        self._get_value_callback = _get_value_ptr_t(_get_value_adapter)
        self._is_smaller_callback = _is_smaller_than_t(_is_smaller_adapter)

        _lib.stack_quick_sort(ctypes.byref(self._stack), self._get_value_callback, should_free, self._is_smaller_callback)

    def close(self):
        if not self._closed:
            _lib.stack_destroy(ctypes.byref(self._stack))
            self._closed = True

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.close()

    def __del__(self):
        # noinspection PyBroadException
        try:
            self.close()
        except:
            # Already closed or library unloaded
            pass


# Export the main API
__all__ = ['Stack']
