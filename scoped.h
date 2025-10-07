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

/* Type definitions */
#define scoped_void_t		__attribute__((cleanup(_SCOPED_void_ptr))) void*
#define scoped_char_t		__attribute__((cleanup(_SCOPED_char_ptr))) char*
#define scoped_short_t		__attribute__((cleanup(_SCOPED_short_ptr))) short*
#define scoped_int_t		__attribute__((cleanup(_SCOPED_int_ptr))) int*
#define scoped_long_t		__attribute__((cleanup(_SCOPED_long_ptr))) long*
#define scoped_float_t		__attribute__((cleanup(_SCOPED_float_ptr))) float*
#define scoped_double_t		__attribute__((cleanup(_SCOPED_double_ptr))) double*
#define scoped_ldouble_t	__attribute__((cleanup(_SCOPED_long_double_ptr))) long double*

#define scoped_uchar_t		__attribute__((cleanup(_SCOPED_unsigned_char_ptr))) unsigned char*
#define scoped_ushort_t		__attribute__((cleanup(_SCOPED_unsigned_short_ptr))) unsigned short*
#define scoped_uint_t		__attribute__((cleanup(_SCOPED_unsigned_int_ptr))) unsigned int*
#define scoped_ulong_t		__attribute__((cleanup(_SCOPED_unsigned_long_ptr))) unsigned long*
#define scoped_ullong_t		__attribute__((cleanup(_SCOPED_unsigned_long_long_ptr))) unsigned long long*

#define scoped_int8_t		__attribute__((cleanup(_SCOPED_int8_t_ptr))) int8_t*
#define scoped_uint8_t		__attribute__((cleanup(_SCOPED_uint8_t_ptr))) uint8_t*
#define scoped_int16_t		__attribute__((cleanup(_SCOPED_int16_t_ptr))) int16_t*
#define scoped_uint16_t		__attribute__((cleanup(_SCOPED_uint16_t_ptr))) uint16_t*
#define scoped_int32_t		__attribute__((cleanup(_SCOPED_int32_t_ptr))) int32_t*
#define scoped_uint32_t		__attribute__((cleanup(_SCOPED_uint32_t_ptr))) uint32_t*
#define scoped_int64_t		__attribute__((cleanup(_SCOPED_int64_t_ptr))) int64_t*
#define scoped_uint64_t		__attribute__((cleanup(_SCOPED_uint64_t_ptr))) uint64_t*

#define scoped_intptr_t		__attribute__((cleanup(_SCOPED_intptr_t_ptr))) intptr_t*
#define scoped_uintptr_t	__attribute__((cleanup(_SCOPED_uintptr_t_ptr))) uintptr_t*

#define scoped_size_t		__attribute__((cleanup(_SCOPED_size_t_ptr))) size_t*
#define scoped_ssize_t		__attribute__((cleanup(_SCOPED_ssize_t_ptr))) ssize_t*
#define scoped_ptrdiff_t	__attribute__((cleanup(_SCOPED_ptrdiff_t_ptr))) ptrdiff_t*

#define scoped_FILE_t		__attribute__((cleanup(_SCOPED_FILE_ptr))) FILE*

/* Registration macro for user-defined pointer types */
#define SCOPED_REGISTER_PTR(T, FUNC)			\
	static inline void _SCOPED_##T##_ptr(T** p)	\
	{											\
		FUNC(*p);								\
	}

/* Public macro for scoped pointer declaration */
#define SCOPED_PTR(T)	__attribute__((cleanup(_SCOPED_##T##_ptr))) T*
#define scoped(T)		SCOPED_PTR(T)

#endif /* SCOPED_H */
