# scoped.h

**scoped.h** is a single-header C library for automatic resource management using GCC's `cleanup` attribute. It enables automatic cleanup for pointers and resources, similar to RAII in C++. This makes resource management easier and less error-prone, especially in complex C code.

## Features

- **Automatic cleanup** of pointers and resources at scope exit
- **Support for standard C pointer types** (e.g., `int*`, `double*`, `FILE*`)
- **Easy registration** of custom cleanup functions for user-defined types
- **Convenient type definitions** for scoped pointers (e.g., `scoped_int_t`, `scoped_file_t`)
- No dependencies other than the C standard library
- Header-only: just include `scoped.h` in your project

## Requirements

- GCC or Clang (support for `__attribute__((cleanup()))`)
- C99 or later

## Usage

1. **Include the header:**

    ```c
    #include "scoped.h"
    ```

2. **Declare scoped pointers with automatic cleanup:**

    There are two main ways to declare scoped pointers:
    
    - Using the type definitions (recommended for common types):  
      For example, `scoped_int_t` for an `int*` that is automatically freed.
    - Using the generic macros:  
      Use the `SCOPED_PTR(T)` or `scoped(T)` macro for any pointer type (including user-defined types with registered cleanup).

### Example: Automatic `free` for heap memory

```c
#include "scoped.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    scoped_int_t x = malloc(sizeof(int)); // Automatically freed
    if (!x) return 1;
    *x = 42;
    printf("x = %d\n", *x);
    // No need to call free(x)! It will be freed automatically.
    return 0;
}
```

### Example: Automatic `fclose` for files

```c
#include "scoped.h"
#include <stdio.h>

int main(void)
{
    scoped_file_t f = fopen("test.txt", "w"); // Automatically fclose'd
    if (!f) return 1;
    fprintf(f, "Hello, scoped file!\n");
    // No need to call fclose(f)! It will be closed automatically.
    return 0;
}
```

### Example: Register cleanup for custom types

Suppose you have a custom struct that needs a specific cleanup routine:

```c
#include "scoped.h"

typedef struct my_type
{
    char* buffer;
} my_type;

void my_type_destroy(my_type* obj)
{
    free(obj->buffer);
}

SCOPED_REGISTER_PTR(my_type, my_type_destroy)

int main(void)
{
    scoped(my_type) obj = malloc(sizeof(my_type)); // Automatically cleaned up
    obj->buffer = malloc(128);
    // No need to call my_type_destroy(obj)! It will be called automatically.
    return 0;
}
```

## Supported Types

- Standard scalar pointer types (`int*`, `double*`, etc.) via type definitions (e.g., `scoped_int_t`, `scoped_double_t`)
- Fixed-width integer pointer types (`int32_t*`, `uint64_t*`, etc.) via type definitions (e.g., `scoped_int32_t`, `scoped_uint64_t`)
- Standard library types (`FILE*`) via `scoped_file_t`
- Custom types via `SCOPED_REGISTER_PTR` and macros (`scoped(T)`)

See `scoped.h` for all predefined type definitions.

## How It Works

The macros and type definitions use GCC/Clang's `__attribute__((cleanup(func)))` extension to automatically call the cleanup function for the pointer variable when it goes out of scope.

## License

scoped.h is released under the **MIT License**. You are free to use, modify, and distribute it under the terms of the license. See the [MIT License](https://opensource.org/licenses/MIT) for more details.

## Author

This header file was developed by **Laurent Mailloux-Bourassa**.
