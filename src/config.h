/*! \file
 * \brief Файл конфигурации проекта.
 */

#ifndef CONFIG_H
#define CONFIG_H


//#define NDEBUG  /* раскомментировать чтобы выключить assert-ы в релизе */
#define QT_ENABLED /* раскомментировать если используется Qt */


#if (defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__ppc64__) || defined(__PPC64__) ||   \
     defined(__64BIT__) || defined(_LP64) || defined(__LP64__) || defined(__ia64) || defined(__itanium__) ||           \
     defined(_M_IA64))
#define ARCHITECTURE_64
#else
#define ARCHITECTURE_32
#endif


#include <cstddef>
#include <cstdint>

#if defined(ARCHITECTURE_64)
using Int  = std::int64_t;
using Uint = std::uint64_t;
#else
using Int  = std::int32_t;
using Uint = std::uint32_t;
#endif

// using Index       = std::size_t;
// using SignedIndex = std::ptrdiff_t;


#endif // CONFIG_H
