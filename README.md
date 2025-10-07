# scoped.h

**scoped.h** is a single-header C library for automatic resource management using GCC's `cleanup` attribute. It enables automatic cleanup for pointers and resources, similar to RAII in C++. This makes managing heap-allocated memory and other resources (like file handles) simpler and less error-prone.

## Features

- **Automatic cleanup** of pointers and resources at scope exit
- **Support for standard C pointer types** (e.g., `int*`, `double*`, `FILE*`)
- **Easy registration** of custom cleanup functions for user-defined types
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

Use the `SCOPED_PTR(T)` or `scoped(T)` macro to declare a pointer of type `T*` that will be automatically cleaned up when it goes out of scope.

### Example: Automatic `free` for heap memory

```c
#include "scoped.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    scoped(int) x = malloc(sizeof(int)); // Automatically freed
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
    scoped(FILE) f = fopen("test.txt", "w"); // Automatically fclose'd
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

- Standard scalar pointer types (`int*`, `double*`, etc.)
- Fixed-width integer pointer types (`int32_t*`, `uint64_t*`, etc.)
- Standard library types (`FILE*`)
- Custom types via `SCOPED_REGISTER_PTR`

See `scoped.h` for all predefined types.

## How It Works

The macros use GCC/Clang's `__attribute__((cleanup(func)))` extension to automatically call the cleanup function for the pointer variable when it goes out of scope.
