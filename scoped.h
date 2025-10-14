/*
 * scoped.h - Scoped Resource Management for C
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
#include <stdint.h>
#include <string.h>

/* Check if we're on a POSIX-compliant system */
#if defined(__unix__) || defined(__unix) ||         \
	(defined(__APPLE__) && defined(__MACH__)) ||    \
	defined(__linux__) || defined(_POSIX_VERSION)
		#define SCOPED_HAS_POSIX 1
#else
	#define SCOPED_HAS_POSIX 0
#endif

/* Check for specific POSIX features */
#if SCOPED_HAS_POSIX
	/* Check if we have unistd.h (file descriptors) */
	#if defined(_POSIX_VERSION) || defined(__unix__) || defined(__linux__)
		#include <unistd.h>
		#define SCOPED_HAS_UNISTD 1
	#else
		#define SCOPED_HAS_UNISTD 0
	#endif

	/* Check if we have socket support */
	#if defined(_POSIX_VERSION) && _POSIX_VERSION >= 200112L
		#include <sys/socket.h>
		#define SCOPED_HAS_SOCKETS 1
	#elif defined(__linux__) || defined(__APPLE__)
		/* Linux and macOS have sockets even if _POSIX_VERSION isn't defined */
		#include <sys/socket.h>
		#define SCOPED_HAS_SOCKETS 1
	#else
		#define SCOPED_HAS_SOCKETS 0
	#endif
#else
	/* Not a POSIX system */
	#define SCOPED_HAS_UNISTD 0
	#define SCOPED_HAS_SOCKETS 0
#endif

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

#if defined(__GNUC__) || defined(__clang__)
    #define _SCOPED(FUNC)   __attribute__((cleanup(FUNC)))
#else
    #warning "scoped.h: cleanup attribute not supported - scoped types will not auto-cleanup"
    #define _SCOPED(FUNC)
#endif

static inline void _SCOPED_free(void* p)
{
    void** ptr = (void**)p;
    if (*ptr)
    {
	    SCOPED_FREE_FUNC(*ptr);
        *ptr = NULL;    // Prevent double-free
    }
}

static inline void _SCOPED_fclose(FILE** f)
{
    if (*f)
    {
        fclose(*f);
        *f = NULL;  // Prevent double-close
    }
}

/* File descriptor support */
#if SCOPED_HAS_UNISTD
static inline void _SCOPED_close(int* fd)
{
    if (*fd >= 0)
    {
        close(*fd);
        *fd = -1;   // Prevent double-close
    }
}
#endif

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
#define _SCOPED_UCHAR_FUNC      _SCOPED_free
#define _SCOPED_USHORT_FUNC     _SCOPED_free
#define _SCOPED_UINT_FUNC       _SCOPED_free
#define _SCOPED_ULONG_FUNC      _SCOPED_free
#define _SCOPED_ULLONG_FUNC     _SCOPED_free

/* Fixed-width integer pointer types */
#define _SCOPED_INT8_FUNC       _SCOPED_free
#define _SCOPED_UINT8_FUNC      _SCOPED_free
#define _SCOPED_INT16_FUNC      _SCOPED_free
#define _SCOPED_UINT16_FUNC     _SCOPED_free
#define _SCOPED_INT32_FUNC      _SCOPED_free
#define _SCOPED_UINT32_FUNC     _SCOPED_free
#define _SCOPED_INT64_FUNC      _SCOPED_free
#define _SCOPED_UINT64_FUNC     _SCOPED_free

/* Pointer-sized integer pointer types */
#define _SCOPED_INTPTR_FUNC     _SCOPED_free
#define _SCOPED_UINTPTR_FUNC    _SCOPED_free

/* Other common standard integer types */
#define _SCOPED_SIZE_FUNC       _SCOPED_free
#define _SCOPED_SSIZE_FUNC      _SCOPED_free
#define _SCOPED_PTRDIFF_FUNC    _SCOPED_free

/* Standard library structs */
#define _SCOPED_FILE_FUNC       _SCOPED_fclose

/* POSIX support */
#if SCOPED_HAS_UNISTD
    #define _SCOPED_FD_FUNC     _SCOPED_close
#endif

#if SCOPED_HAS_SOCKETS
    #define _SCOPED_SOCKET_FUNC _SCOPED_close
#endif

/* Type definitions */
#define scoped_void_p		_SCOPED(_SCOPED_VOID_FUNC)      void*
#define scoped_char_p		_SCOPED(_SCOPED_CHAR_FUNC)      char*
#define scoped_short_p		_SCOPED(_SCOPED_SHORT_FUNC)     short*
#define scoped_int_p		_SCOPED(_SCOPED_INT_FUNC)       int*
#define scoped_long_p		_SCOPED(_SCOPED_LONG_FUNC)      long*
#define scoped_float_p		_SCOPED(_SCOPED_FLOAT_FUNC)     float*
#define scoped_double_p		_SCOPED(_SCOPED_DOUBLE_FUNC)    double*
#define scoped_ldouble_p    _SCOPED(_SCOPED_LDOUBLE_FUNC)   long double*

#define scoped_uchar_p		_SCOPED(_SCOPED_UCHAR_FUNC)     unsigned char*
#define scoped_ushort_p		_SCOPED(_SCOPED_USHORT_FUNC)    unsigned short*
#define scoped_uint_p		_SCOPED(_SCOPED_UINT_FUNC)      unsigned int*
#define scoped_ulong_p		_SCOPED(_SCOPED_ULONG_FUNC)     unsigned long*
#define scoped_ullong_p		_SCOPED(_SCOPED_ULLONG_FUNC)    unsigned long long*

#define scoped_int8_p		_SCOPED(_SCOPED_INT8_FUNC)      int8_t*
#define scoped_uint8_p		_SCOPED(_SCOPED_UINT8_FUNC)     uint8_t*
#define scoped_int16_p		_SCOPED(_SCOPED_INT16_FUNC)     int16_t*
#define scoped_uint16_p		_SCOPED(_SCOPED_UINT16_FUNC)    uint16_t*
#define scoped_int32_p		_SCOPED(_SCOPED_INT32_FUNC)     int32_t*
#define scoped_uint32_p		_SCOPED(_SCOPED_UINT32_FUNC)    uint32_t*
#define scoped_int64_p		_SCOPED(_SCOPED_INT64_FUNC)     int64_t*
#define scoped_uint64_p		_SCOPED(_SCOPED_UINT64_FUNC)    uint64_t*

#define scoped_intptr_p		_SCOPED(_SCOPED_INTPTR_FUNC)    intptr_t*
#define scoped_uintptr_p	_SCOPED(_SCOPED_UINTPTR_FUNC)   uintptr_t*

#define scoped_size_p		_SCOPED(_SCOPED_SIZE_FUNC)      size_t*
#define scoped_ssize_p		_SCOPED(_SCOPED_SSIZE_FUNC)     ssize_t*
#define scoped_ptrdiff_p	_SCOPED(_SCOPED_PTRDIFF_FUNC)   ptrdiff_t*

#define scoped_file_p		_SCOPED(_SCOPED_FILE_FUNC)      FILE*

/* POSIX types */
#if SCOPED_HAS_UNISTD
    #define scoped_fd       _SCOPED(_SCOPED_FD_FUNC)        int
#endif

#if SCOPED_HAS_SOCKETS
    #define scoped_socket   _SCOPED(_SCOPED_SOCKET_FUNC)    int
#endif

/* Helper macros */

/* Registration macro for user-defined types */
#define SCOPED_REGISTER_CUSTOM_TYPE(T, FUNC)        \
	static inline void _SCOPED_##T##_CUSTOM(T* p)   \
	{											    \
        FUNC(p);                                    \
	}

/* Registration macro for user-defined pointer types */
#define SCOPED_REGISTER_CUSTOM_TYPE_PTR(T, FUNC)        \
	static inline void _SCOPED_##T##_PTR_CUSTOM(T** p)  \
	{											        \
        if (*p)                                         \
        {                                               \
		    FUNC(*p);                                   \
            *p = NULL;                                  \
        }                                               \
	}

/* Public macro for scoped user-defined type declaration */
#define scoped(T)   _SCOPED(_SCOPED_##T##_CUSTOM) T

/* Public macro for scoped user-defined type pointer declaration */
#define scoped_p(T) _SCOPED(_SCOPED_##T##_PTR_CUSTOM) T*

/**
 * Transfer ownership from one scoped variable to another
 * Sets source to NULL to prevent double-free
 * 
 * Example:
 *   scoped_int_t a = scoped_malloc(int, 10);
 *   scoped_int_t b = NULL;
 *   SCOPED_TRANSFER(b, a); // now b owns the memory, a is NULL
 */
#define SCOPED_TRANSFER(dest, src)  \
    do {                            \
        (dest) = (src);             \
        (src) = NULL;               \
    } while(0)

/**
 * Transfer ownership from raw pointer to scoped variable
 * Sets source to NULL to prevent double-free
 * 
 * Example:
 *   int* raw = malloc(10 * sizeof(int));
 *   scoped_int_t scoped_arr = NULL;
 *   SCOPED_TAKE_OWNERSHIP(scoped_arr, raw); // now scoped_arr owns the memory
 */
#define SCOPED_TAKE_OWNERSHIP(scoped_var, raw_ptr)  \
    do {                                            \
        (scoped_var) = (raw_ptr);                   \
        (raw_ptr) = NULL;                           \
    } while(0)

/**
 * Release ownership from scoped variable to raw pointer
 * Sets scoped variable to NULL to prevent cleanup
 * 
 * Example:
 *   scoped_int_t scoped_arr = scoped_malloc(int, 10);
 *   int* raw = SCOPED_RELEASE(scoped_arr); // now you must manually free(raw)
 */
#define SCOPED_RELEASE(scoped_var)                              \
    ({                                                          \
        __typeof__(*(scoped_var))* _released = (scoped_var);    \
        (scoped_var) = NULL;                                    \
        _released;                                              \
    })

/**
 * Safe malloc using custom allocator
 * 
 * Example:
 *   scoped_int_t arr = scoped_malloc(int, 10);
 *   // arr is automatically freed when it goes out of scope
 */
#define scoped_malloc(T, count)                             \
    ({                                                      \
        T* _ptr = SCOPED_MALLOC_FUNC((count) * sizeof(T));  \
        _ptr;                                               \
    })

/**
 * Safe calloc using custom allocator
 * 
 * Example:
 *   scoped_int_t arr = scoped_calloc(int, 10);
 */
#define scoped_calloc(T, count)                             \
    ({                                                      \
        T* _ptr = SCOPED_CALLOC_FUNC((count), sizeof(T));   \
        _ptr;                                               \
    })

/**
 * Safe realloc using custom allocator
 * 
 * Note: If realloc fails, the original pointer remains unchanged
 * 
 * Example:
 *   scoped_int_t arr = scoped_malloc(int, 10);
 *   if (!scoped_realloc(arr, 20))
 *   {
 *       // Handle allocation failure - original memory still valid
 *   }
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
