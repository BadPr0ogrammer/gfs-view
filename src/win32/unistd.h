#ifndef _UNISTD_H
#define _UNISTD_H 1

#ifndef _MSC_VER // [
#error "Use this header only with Microsoft Visual C++ compilers!"
#endif // _MSC_VER ]

#include <limits.h>
#ifdef __cplusplus
extern "C" {
#endif
#  include <wchar.h>
#ifdef __cplusplus
}
#endif

/* This file intended to serve as a drop-in replacement for

    unistd.h on Windows
    Please add functionality as neeeded */

/* #include
#include
#include
#include */  /* for getpid() and the exec..() family /
#include / for _getcwd() and _chdir() */

#define srandom srand
#define random rand

/* Values for the second argument to access.
These may be OR'd together. /
#define R_OK 4 / Test for read permission. /
#define W_OK 2 / Test for write permission. /
//#define X_OK 1 / execute permission - unsupported in windows/
#define F_OK 0 / Test for existence. */

#define access _access
#define dup2 _dup2
#define execve _execve
#define ftruncate _chsize
#define unlink _unlink
#define fileno _fileno
#define getcwd _getcwd
#define chdir _chdir
#define isatty _isatty
#define lseek _lseek
/* read, write, and close are NOT being #defined here, because while there are file handle specific versions for
Windows, they probably don't work for sockets. You need to look at your app and consider whether to call e.g.
closesocket(). */

#define ssize_t int

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2
/* should be in some equivalent to */
typedef signed __int8 int8_t;
typedef signed __int16 int16_t;
typedef signed __int32 int32_t;
typedef signed __int64 int64_t;
typedef unsigned __int8 uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;

// 7.18.1.4 Integer types capable of holding object pointers
#ifdef _WIN64 // [
   typedef signed __int64    intptr_t;
   typedef unsigned __int64  uintptr_t;
#else // _WIN64 ][
   typedef _W64 signed int   intptr_t;
   typedef _W64 unsigned int uintptr_t;
#endif // _WIN64 ]
// 7.18.1.5 Greatest-width integer types
typedef int64_t   intmax_t;
typedef uint64_t  uintmax_t;


#endif
