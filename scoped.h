/*
 * scoped.h
 *
 * Copyright (c) 2025 Laurent Mailloux-Bourassa
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef SCOPED_H
#define SCOPED_H

#include <stdlib.h>
#include <stdio.h>

/* Allow user to override the default malloc function */
#ifndef SCOPED_MALLOC_FUNC
	#define SCOPED_MALLOC_FUNC  malloc
#endif

/* Allow user to override the default calloc function */
#ifndef SCOPED_CALLOC_FUNC
	#define SCOPED_CALLOC_FUNC  calloc
#endif

/* Allow user to override the default realloc function */
#ifndef SCOPED_REALLOC_FUNC
	#define SCOPED_REALLOC_FUNC realloc
#endif

/* Allow user to override the default free function */
#ifndef SCOPED_FREE_FUNC
	#define SCOPED_FREE_FUNC    free
#endif

static inline void _SCOPED_free(void* p)
{
	SCOPED_FREE_FUNC(*(void**) p);
}

static inline void _SCOPED_fclose(FILE** f)
{
	fclose(*f);
}

/* Core C scalar pointer types */
#define _SCOPED_VOID_FUNC       _SCOPED_free
#define _SCOPED_CHAR_FUNC       _SCOPED_free
#define _SCOPED_SHORT_FUNC      _SCOPED_free
#define _SCOPED_INT_FUNC        _SCOPED_free
#define _SCOPED_LONG_FUNC       _SCOPED_free
#define _SCOPED_FLOAT_FUNC      _SCOPED_free
#define _SCOPED_DOUBLE_FUNC     _SCOPED_free
#define _SCOPED_LDOUBLE_FUNC    _SCOPED_free

/* Unsigned scalar pointer types */
#define _SCOPED_UCHAR_FUNC  _SCOPED_free
#define _SCOPED_USHORT_FUNC _SCOPED_free
#define _SCOPED_UINT_FUNC   _SCOPED_free
#define _SCOPED_ULONG_FUNC  _SCOPED_free
#define _SCOPED_ULLONG_FUNC _SCOPED_free

/* Fixed-width integer pointer types */
#define _SCOPED_INT8_FUNC   _SCOPED_free
#define _SCOPED_UINT8_FUNC  _SCOPED_free
#define _SCOPED_INT16_FUNC  _SCOPED_free
#define _SCOPED_UINT16_FUNC _SCOPED_free
#define _SCOPED_INT32_FUNC  _SCOPED_free
#define _SCOPED_UINT32_FUNC _SCOPED_free
#define _SCOPED_INT64_FUNC  _SCOPED_free
#define _SCOPED_UINT64_FUNC _SCOPED_free

/* Pointer-sized integer pointer types */
#define _SCOPED_INTPTR_FUNC     _SCOPED_free
#define _SCOPED_UINTPTR_FUNC    _SCOPED_free

/* Other common standard integer types */
#define _SCOPED_SIZE_FUNC       _SCOPED_free
#define _SCOPED_SSIZE_FUNC      _SCOPED_free
#define _SCOPED_PTRDIFF_FUNC    _SCOPED_free

/* Standard library structs */
#define _SCOPED_FILE_FUNC	_SCOPED_fclose

/* Type definitions */
#define scoped_void_t		__attribute__((cleanup(_SCOPED_VOID_FUNC))) void*
#define scoped_char_t		__attribute__((cleanup(_SCOPED_CHAR_FUNC))) char*
#define scoped_short_t		__attribute__((cleanup(_SCOPED_SHORT_FUNC))) short*
#define scoped_int_t		__attribute__((cleanup(_SCOPED_INT_FUNC))) int*
#define scoped_long_t		__attribute__((cleanup(_SCOPED_LONG_FUNC))) long*
#define scoped_float_t		__attribute__((cleanup(_SCOPED_FLOAT_FUNC))) float*
#define scoped_double_t		__attribute__((cleanup(_SCOPED_DOUBLE_FUNC))) double*
#define scoped_ldouble_t	__attribute__((cleanup(_SCOPED_LDOUBLE_FUNC))) long double*

#define scoped_uchar_t		__attribute__((cleanup(_SCOPED_UCHAR_FUNC))) unsigned char*
#define scoped_ushort_t		__attribute__((cleanup(_SCOPED_USHORT_FUNC))) unsigned short*
#define scoped_uint_t		__attribute__((cleanup(_SCOPED_UINT_FUNC))) unsigned int*
#define scoped_ulong_t		__attribute__((cleanup(_SCOPED_ULONG_FUNC))) unsigned long*
#define scoped_ullong_t		__attribute__((cleanup(_SCOPED_ULLONG_FUNC))) unsigned long long*

#define scoped_int8_t		__attribute__((cleanup(_SCOPED_INT8_FUNC))) int8_t*
#define scoped_uint8_t		__attribute__((cleanup(_SCOPED_UINT8_FUNC))) uint8_t*
#define scoped_int16_t		__attribute__((cleanup(_SCOPED_INT16_FUNC))) int16_t*
#define scoped_uint16_t		__attribute__((cleanup(_SCOPED_UINT16_FUNC))) uint16_t*
#define scoped_int32_t		__attribute__((cleanup(_SCOPED_INT32_FUNC))) int32_t*
#define scoped_uint32_t		__attribute__((cleanup(_SCOPED_UINT32_FUNC))) uint32_t*
#define scoped_int64_t		__attribute__((cleanup(_SCOPED_INT64_FUNC))) int64_t*
#define scoped_uint64_t		__attribute__((cleanup(_SCOPED_UINT64_FUNC))) uint64_t*

#define scoped_intptr_t		__attribute__((cleanup(_SCOPED_INTPTR_FUNC))) intptr_t*
#define scoped_uintptr_t	__attribute__((cleanup(_SCOPED_UINTPTR_FUNC))) uintptr_t*

#define scoped_size_t		__attribute__((cleanup(_SCOPED_SIZE_FUNC))) size_t*
#define scoped_ssize_t		__attribute__((cleanup(_SCOPED_SSIZE_FUNC))) ssize_t*
#define scoped_ptrdiff_t	__attribute__((cleanup(_SCOPED_PTRDIFF_FUNC))) ptrdiff_t*

#define scoped_file_t		__attribute__((cleanup(_SCOPED_FILE_FUNC))) FILE*

/* Registration macro for user-defined pointer types */
#define SCOPED_REGISTER_CUSTOM_TYPE(T, FUNC)        \
	static inline void _SCOPED_##T##_CUSTOM(T** p)  \
	{											    \
		FUNC(*p);								    \
	}

/* Public macro for scoped user-defined type pointer declaration */
#define scoped(T)   __attribute__((cleanup(_SCOPED_##T##_CUSTOM))) T*

/* Helper macros */

/**
 * Transfer ownership from one scoped variable to another
 * Sets source to NULL to prevent double-free
 */
#define SCOPED_TRANSFER(dest, src)  \
    do {                            \
        (dest) = (src);             \
        (src) = NULL;               \
    } while(0)

/**
 * Transfer ownership from raw pointer to scoped variable
 * Sets source to NULL to prevent double-free
 */
#define SCOPED_TAKE_OWNERSHIP(scoped_var, raw_ptr)  \
    do {                                            \
        (scoped_var) = (raw_ptr);                   \
        (raw_ptr) = NULL;                           \
    } while(0)

/**
 * Release ownership from scoped variable to raw pointer
 * Sets scoped variable to NULL to prevent cleanup
 */
#define SCOPED_RELEASE(scoped_var)                              \
    ({                                                          \
        __typeof__(*(scoped_var))* _released = (scoped_var);    \
        (scoped_var) = NULL;                                    \
        _released;                                              \
    })

/**
 * Safe malloc with automatic zero-initialization using custom allocator
 */
#define scoped_malloc(T, count)                             \
    ({                                                      \
        T* _ptr = SCOPED_MALLOC_FUNC((count) * sizeof(T));  \
        if (_ptr) memset(_ptr, 0, (count) * sizeof(T));     \
        _ptr;                                               \
    })

/**
 * Safe calloc using custom allocator
 */
#define scoped_calloc(T, count)                             \
    ({                                                      \
        T* _ptr = SCOPED_CALLOC_FUNC((count), sizeof(T));   \
        _ptr;                                               \
    })

/**
 * Safe realloc that preserves the original pointer for cleanup
 * Returns 1 on success, 0 on failure
 */
#define scoped_realloc(scoped_var, new_count)                                                               \
    ({                                                                                                      \
        __typeof__(*(scoped_var))* _old_ptr = (scoped_var);                                                 \
        __typeof__(*_old_ptr)* _new_ptr = SCOPED_REALLOC_FUNC(_old_ptr, (new_count) * sizeof(*_old_ptr));   \
        if (_new_ptr)                                                                                       \
        {                                                                                                   \
            (scoped_var) = _new_ptr;                                                                        \
        }                                                                                                   \
        _new_ptr;                                                                                           \
    })

#endif /* SCOPED_H */
