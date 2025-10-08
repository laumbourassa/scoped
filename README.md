# scoped.h

**scoped.h** is a single-header C library for automatic resource management using GCC/Clang's `cleanup` attribute. It enables automatic cleanup for pointers and resources, similar to RAII in C++. This makes resource leaks less likely, and your code cleaner and safer.

## Features

- **Automatic cleanup** of pointers and resources at scope exit
- **Support for standard C pointer types** (e.g., `int*`, `double*`, `FILE*`)
- **Support for POSIX resources** (file descriptors, sockets) on compatible platforms
- **Easy registration** of custom cleanup functions for user-defined types
- **Convenient type definitions** for scoped pointers (e.g., `scoped_int_t`, `scoped_file_t`)
- **Ownership transfer macros** (`SCOPED_TRANSFER`, `SCOPED_TAKE_OWNERSHIP`, `SCOPED_RELEASE`)
- **Custom allocator support** (override malloc/calloc/realloc/free)
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

    - Using the provided type definitions (recommended for common types):  
      For example, `scoped_int_t` for an `int*` that is automatically freed.
    - Using the generic macro for custom types:  
      Use the `scoped(T)` macro for any pointer type (after registering a cleanup function).

### Example: Automatic `free` for heap memory

```c
#include "scoped.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    scoped_int_t x = scoped_malloc(int, 1); // Automatically freed
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

### Example: Automatic close for POSIX file descriptors

```c
#include "scoped.h"
#if SCOPED_HAS_UNISTD
#include <unistd.h>
#include <fcntl.h>
#endif

int main(void)
{
#if SCOPED_HAS_UNISTD
    scoped_fd_t fd = open("test.txt", O_CREAT | O_WRONLY, 0644); // Automatically closed
    if (fd < 0) return 1;
    write(fd, "Hello, scoped fd!\n", 18);
    // No need to call close(fd)! It will be closed automatically.
#endif
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

SCOPED_REGISTER_CUSTOM_TYPE(my_type, my_type_destroy)

int main(void)
{
    scoped(my_type) obj = scoped_malloc(my_type, 1); // Automatically cleaned up
    obj->buffer = scoped_malloc(char, 128);
    // No need to call my_type_destroy(obj)! It will be called automatically.
    return 0;
}
```

### Ownership Transfer Macros

- **Transfer ownership between scoped variables:**

    ```c
    scoped_int_t a = scoped_malloc(int, 10);
    scoped_int_t b = NULL;
    SCOPED_TRANSFER(b, a); // b owns the memory, a is NULL
    ```

- **Take ownership from raw pointer:**

    ```c
    int* raw = malloc(10 * sizeof(int));
    scoped_int_t scoped_arr = NULL;
    SCOPED_TAKE_OWNERSHIP(scoped_arr, raw); // scoped_arr owns memory, raw is NULL
    ```

- **Release ownership to raw pointer:**

    ```c
    scoped_int_t scoped_arr = scoped_malloc(int, 10);
    int* raw = SCOPED_RELEASE(scoped_arr); // you must manually free(raw)
    ```

### Custom Allocator Support

You can override the default memory functions by defining macros before including `scoped.h`:

```c
#define SCOPED_MALLOC_FUNC  my_malloc
#define SCOPED_FREE_FUNC    my_free
#include "scoped.h"
```

## Supported Types

- Standard scalar pointer types (`int*`, `double*`, etc.) via type definitions (e.g., `scoped_int_t`, `scoped_double_t`)
- Fixed-width integer pointer types (`int32_t*`, `uint64_t*`, etc.) via type definitions (e.g., `scoped_int32_t`, `scoped_uint64_t`)
- Standard library types (`FILE*`) via `scoped_file_t`
- POSIX resources (`scoped_fd_t`, `scoped_socket_t`) on supported platforms
- Custom types via `SCOPED_REGISTER_CUSTOM_TYPE` and macros (`scoped(T)`)

See `scoped.h` for all predefined type definitions.

## How It Works

The macros and type definitions use GCC/Clang's `__attribute__((cleanup(func)))` extension to automatically call the cleanup function for the pointer variable when it goes out of scope. If your compiler doesn't support this attribute, the macros will have no effect and a warning is issued.

## License

scoped.h is released under the **MIT License**. You are free to use, modify, and distribute it under the terms of the license. See the [MIT License](https://opensource.org/licenses/MIT) for more details.

## Author

This header file was developed by **Laurent Mailloux-Bourassa**.
