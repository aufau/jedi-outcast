#ifndef __Q_PLATFORM_H
#define __Q_PLATFORM_H

// this is the define for determining if we have an asm version of a C function
#define idx64 0

#ifdef Q3_VM

#define id386 0
#define idppc 0
#define idppc_altivec 0
#define idsparc 0

#else

#if (defined _M_IX86 || defined __i386__) && !defined(C_ONLY)
#define id386	1
#else
#define id386	0
#endif

#if (defined(powerc) || defined(powerpc) || defined(ppc) || \
     defined(__ppc) || defined(__ppc__)) && !defined(C_ONLY)
#define idppc	1
#else
#define idppc	0
#endif

#endif // Q3_VM

#ifndef __ASM_I386__ // don't include the C bits if included from qasm.h

// for windows fastcall option
#define	QDECL

//======================= WIN32 DEFINES =================================

#if defined(_WIN32) || defined(__WIN32__)

#undef QDECL
#define	QDECL __cdecl

#if defined( _MSC_VER )
#define OS_STRING "win_msvc"
#elif defined __MINGW32__
#define OS_STRING "win_mingw"
#endif

#define ID_INLINE __inline 
#define PATH_SEP '\\'

#if defined( _M_IX86 ) || defined( __i386__ )
#define ARCH_STRING "x86"
#elif defined _M_ALPHA
#define ARCH_STRING "AXP"
#endif

#define Q3_LITTLE_ENDIAN
#define DLL_EXT ".dll"

#endif

//======================= LINUX DEFINES =================================

#if defined(__linux__) || defined(__FreeBSD_kernel__)

#include <endian.h>

#if defined(__linux__)
#define OS_STRING "linux"
#else
#define OS_STRING "kFreeBSD"
#endif

#define ID_INLINE inline

#define PATH_SEP '/'

#if defined __i386__
#define ARCH_STRING "x86"
#elif defined __x86_64__
//#undef idx64
//#define idx64 1
#define ARCH_STRING "x86_64"
#elif defined __powerpc64__
#define ARCH_STRING "ppc64"
#elif defined __powerpc__
#define ARCH_STRING "ppc"
#elif defined __s390__
#define ARCH_STRING "s390"
#elif defined __s390x__
#define ARCH_STRING "s390x"
#elif defined __ia64__
#define ARCH_STRING "ia64"
#elif defined __alpha__
#define ARCH_STRING "alpha"
#elif defined __sparc__
#define ARCH_STRING "sparc"
#elif defined __arm__
#define ARCH_STRING "arm"
#elif defined __cris__
#define ARCH_STRING "cris"
#elif defined __hppa__
#define ARCH_STRING "hppa"
#elif defined __mips__
#define ARCH_STRING "mips"
#elif defined __sh__
#define ARCH_STRING "sh"
#endif

#if __FLOAT_WORD_ORDER == __BIG_ENDIAN
#define Q3_BIG_ENDIAN
#else
#define Q3_LITTLE_ENDIAN
#endif

#define DLL_EXT ".so"

#endif

//========================= BSD DEFINES =================================

#if defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)

#include <sys/types.h>
#include <machine/endian.h>

#ifndef __BSD__
  #define __BSD__
#endif

#if defined(__FreeBSD__)
#define OS_STRING "freebsd"
#elif defined(__OpenBSD__)
#define OS_STRING "openbsd"
#elif defined(__NetBSD__)
#define OS_STRING "netbsd"
#endif

#define ID_INLINE inline
#define PATH_SEP '/'

#ifdef __i386__
#define ARCH_STRING "x86"
#elif defined __amd64__
//#undef idx64
//#define idx64 1
#define ARCH_STRING "x86_64"
#elif defined __axp__
#define ARCH_STRING "alpha"
#endif

#if BYTE_ORDER == BIG_ENDIAN
#define Q3_BIG_ENDIAN
#else
#define Q3_LITTLE_ENDIAN
#endif

#define DLL_EXT ".so"

#endif

//================================================================= SUNOS ===

#ifdef __sun

#include <stdint.h>
#include <sys/byteorder.h>

#define OS_STRING "solaris"
#define ID_INLINE inline
#define PATH_SEP '/'

#ifdef __i386__
#define ARCH_STRING "x86"
#elif defined __sparc
#define ARCH_STRING "sparc"
#endif

#if defined( _BIG_ENDIAN )
#define Q3_BIG_ENDIAN
#elif defined( _LITTLE_ENDIAN )
#define Q3_LITTLE_ENDIAN
#endif

#define DLL_EXT ".so"

#endif

//================================================================== IRIX ===

#ifdef __sgi

#define OS_STRING "irix"
#define ID_INLINE __inline
#define PATH_SEP '/'

#define ARCH_STRING "mips"

#define Q3_BIG_ENDIAN // SGI's MIPS are always big endian

#define DLL_EXT ".so"

#endif

//================================================================== Q3VM ===

#ifdef Q3_VM

#define OS_STRING "q3vm"
#define ID_INLINE
#define PATH_SEP '/'

#define ARCH_STRING "bytecode"

#define DLL_EXT ".qvm"

#endif

//===========================================================================

//catch missing defines in above blocks
#if !defined( OS_STRING )
#error "Operating system not supported"
#endif

#if !defined( ARCH_STRING )
#error "Architecture not supported"
#endif

#ifndef ID_INLINE
#error "ID_INLINE not defined"
#endif

#ifndef PATH_SEP
#error "PATH_SEP not defined"
#endif

#ifndef DLL_EXT
#error "DLL_EXT not defined"
#endif

//endianness
short	ShortSwap (short l);
int	LongSwap (int l);
float	FloatSwap (const float *f);

#if defined( Q3_BIG_ENDIAN ) && defined( Q3_LITTLE_ENDIAN )
#error "Endianness defined as both big and little"
#elif defined( Q3_BIG_ENDIAN )

#define LittleShort(x) ShortSwap(x)
#define LittleLong(x) LongSwap(x)
#define LittleFloat(x) FloatSwap(&x)
#define BigShort
#define BigLong
#define BigFloat

#elif defined( Q3_LITTLE_ENDIAN )

#define LittleShort
#define LittleLong
#define LittleFloat
#define BigShort(x) ShortSwap(x)
#define BigLong(x) LongSwap(x)
#define BigFloat(x) FloatSwap(&x)

#elif defined( Q3_VM )

#define LittleShort
#define LittleLong
#define LittleFloat
#define BigShort
#define BigLong
#define BigFloat

#else
#error "Endianness not defined"
#endif

//platform string
#ifdef NDEBUG
#define PLATFORM_STRING OS_STRING "-" ARCH_STRING
#else
#define PLATFORM_STRING OS_STRING "-" ARCH_STRING "-debug"
#endif

#endif // __ASM_I386__

#endif // __Q_PLATFORM_H
