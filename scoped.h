#ifndef SCOPED_H
#define SCOPED_H

#include <stdlib.h>
#include <stdio.h>

static inline void _SCOPED_free(void* p)
{
	free(*(void**) p);
}

static inline void _SCOPED_fclose(FILE** f)
{
	fclose(*f);
}

/* Core C scalar pointer types */
#define _SCOPED_void_ptr		_SCOPED_free
#define _SCOPED_char_ptr		_SCOPED_free
#define _SCOPED_short_ptr		_SCOPED_free
#define _SCOPED_int_ptr			_SCOPED_free
#define _SCOPED_long_ptr		_SCOPED_free
#define _SCOPED_float_ptr		_SCOPED_free
#define _SCOPED_double_ptr		_SCOPED_free
#define _SCOPED_long_double_ptr	_SCOPED_free

/* Unsigned scalar pointer types */
#define _SCOPED_unsigned_char_ptr		_SCOPED_free
#define _SCOPED_unsigned_short_ptr		_SCOPED_free
#define _SCOPED_unsigned_int_ptr		_SCOPED_free
#define _SCOPED_unsigned_long_ptr		_SCOPED_free
#define _SCOPED_unsigned_long_long_ptr	_SCOPED_free

/* Fixed-width integer pointer types */
#define _SCOPED_int8_t_ptr		_SCOPED_free
#define _SCOPED_uint8_t_ptr		_SCOPED_free
#define _SCOPED_int16_t_ptr		_SCOPED_free
#define _SCOPED_uint16_t_ptr	_SCOPED_free
#define _SCOPED_int32_t_ptr		_SCOPED_free
#define _SCOPED_uint32_t_ptr	_SCOPED_free
#define _SCOPED_int64_t_ptr		_SCOPED_free
#define _SCOPED_uint64_t_ptr	_SCOPED_free

/* Pointer-sized integer pointer types */
#define _SCOPED_intptr_t_ptr	_SCOPED_free
#define _SCOPED_uintptr_t_ptr	_SCOPED_free

/* Other common standard integer types */
#define _SCOPED_size_t_ptr		_SCOPED_free
#define _SCOPED_ssize_t_ptr		_SCOPED_free
#define _SCOPED_ptrdiff_t_ptr	_SCOPED_free

/* Standard library structs */
#define _SCOPED_FILE_ptr	_SCOPED_fclose

/* Registration macro for user-defined pointer types */
#define SCOPED_REGISTER_PTR(T, FUNC)			\
    static inline void _SCOPED_##T##_ptr(T** p)	\
    {											\
        if (*p) FUNC(*p);						\
    }

/* Public macro for scoped pointer declaration */
#define SCOPED_PTR(T)	__attribute__((cleanup(_SCOPED_##T##_ptr))) T*
#define scoped(T)		SCOPED_PTR(T)

#endif /* SCOPED_H */
