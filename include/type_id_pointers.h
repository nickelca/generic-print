#ifndef TYPE_ID_POINTERS_H
#define TYPE_ID_POINTERS_H

#include <stddef.h>
#include <stdint.h>
#include <wchar.h>

#include "type_id_common.h"

// clang-format off
#define TYPE_ID_CVA_POINTERS(T, ID)                                            \
    T *: TIDP(ID),                                                             \
    const T *: TIDP(ID),                                                       \
    volatile T *: TIDP(ID),                                                    \
    const volatile T *: TIDP(ID),                                              \
    _Atomic T *: TIDP(ID),                                                     \
    const _Atomic T *: TIDP(ID),                                               \
    const volatile _Atomic T *: TIDP(ID)
// clang-format on

#define TYPE_ID_POINTERS_INTN_LEAST_TYPES(x)                                   \
    _Generic(                                                                  \
        (typeof_unqual(x)){},                                                  \
        TYPE_ID_CVA_POINTERS(int_least8_t, Int_Least8),                        \
        TYPE_ID_CVA_POINTERS(uint_least8_t, UInt_Least8),                      \
        default: _Generic(                                                     \
            (typeof_unqual(x)){},                                              \
            TYPE_ID_CVA_POINTERS(int_least16_t, Int_Least16),                  \
            TYPE_ID_CVA_POINTERS(uint_least16_t, UInt_Least16),                \
            default: _Generic(                                                 \
                (typeof_unqual(x)){},                                          \
                TYPE_ID_CVA_POINTERS(int_least32_t, Int_Least32),              \
                TYPE_ID_CVA_POINTERS(uint_least32_t, UInt_Least32),            \
                default: _Generic(                                             \
                    (typeof_unqual(x)){},                                      \
                    TYPE_ID_CVA_POINTERS(int_least64_t, Int_Least64),          \
                    TYPE_ID_CVA_POINTERS(uint_least64_t, UInt_Least64),        \
                    default: TYPE_ID_POINTERS_DEFAULT(x)                       \
                )                                                              \
            )                                                                  \
        )                                                                      \
    )

#define TYPE_ID_POINTERS_INTN_FAST_TYPES(x)                                    \
    _Generic(                                                                  \
        (typeof_unqual(x)){},                                                  \
        TYPE_ID_CVA_POINTERS(int_fast8_t, Int_Fast8),                          \
        TYPE_ID_CVA_POINTERS(uint_fast8_t, UInt_Fast8),                        \
        default: _Generic(                                                     \
            (typeof_unqual(x)){},                                              \
            TYPE_ID_CVA_POINTERS(int_fast16_t, Int_Fast16),                    \
            TYPE_ID_CVA_POINTERS(uint_fast16_t, UInt_Fast16),                  \
            default: _Generic(                                                 \
                (typeof_unqual(x)){},                                          \
                TYPE_ID_CVA_POINTERS(int_fast32_t, Int_Fast32),                \
                TYPE_ID_CVA_POINTERS(uint_fast32_t, UInt_Fast32),              \
                default: _Generic(                                             \
                    (typeof_unqual(x)){},                                      \
                    TYPE_ID_CVA_POINTERS(int_fast64_t, Int_Fast64),            \
                    TYPE_ID_CVA_POINTERS(uint_fast64_t, UInt_Fast64),          \
                    default: TYPE_ID_POINTERS_INTN_LEAST_TYPES(x)              \
                )                                                              \
            )                                                                  \
        )                                                                      \
    )

#ifdef INT64_MAX
#    define TYPE_ID_POINTERS_INT64_TYPES(x)                                    \
        _Generic(                                                              \
            (typeof_unqual(x)){},                                              \
            TYPE_ID_CVA_POINTERS(int64_t, Int64),                              \
            TYPE_ID_CVA_POINTERS(uint64_t, UInt64),                            \
            default: TYPE_ID_POINTERS_INTN_FAST_TYPES(x)                       \
        )
#else
#    define TYPE_ID_POINTERS_INT64_TYPES(x) TYPE_ID_POINTERS_INTN_FAST_TYPES(x)
#endif

#ifdef INT32_MAX
#    define TYPE_ID_POINTERS_INT32_TYPES(x)                                    \
        _Generic(                                                              \
            (typeof_unqual(x)){},                                              \
            TYPE_ID_CVA_POINTERS(int32_t, Int32),                              \
            TYPE_ID_CVA_POINTERS(uint32_t, UInt32),                            \
            default: TYPE_ID_POINTERS_INT64_TYPES(x)                           \
        )
#else
#    define TYPE_ID_POINTERS_INT32_TYPES(x) TYPE_ID_POINTERS_INT64_TYPES(x)
#endif

#ifdef INT16_MAX
#    define TYPE_ID_POINTERS_INT16_TYPES(x)                                    \
        _Generic(                                                              \
            (typeof_unqual(x)){},                                              \
            TYPE_ID_CVA_POINTERS(int16_t, Int16),                              \
            TYPE_ID_CVA_POINTERS(uint16_t, UInt16),                            \
            default: TYPE_ID_POINTERS_INT32_TYPES(x)                           \
        )
#else
#    define TYPE_ID_POINTERS_INT16_TYPES(x) TYPE_ID_POINTERS_INT32_TYPES(x)
#endif

#ifdef INT8_MAX
#    define TYPE_ID_POINTERS_INT8_TYPES(x)                                     \
        _Generic(                                                              \
            (typeof_unqual(x)){},                                              \
            TYPE_ID_CVA_POINTERS(int8_t, Int8),                                \
            TYPE_ID_CVA_POINTERS(uint8_t, UInt8),                              \
            default: TYPE_ID_POINTERS_INT16_TYPES(x)                           \
        )
#else
#    define TYPE_ID_POINTERS_INT8_TYPES(x) TYPE_ID_POINTERS_INT16_TYPES(x)
#endif

#ifdef INTPTR_MAX
#    define TYPE_ID_POINTERS_INTPTR_TYPES(x)                                   \
        _Generic(                                                              \
            (typeof_unqual(x)){},                                              \
            TYPE_ID_CVA_POINTERS(intptr_t, Intptr),                            \
            TYPE_ID_CVA_POINTERS(uintptr_t, UIntptr),                          \
            default: TYPE_ID_POINTERS_INT8_TYPES(x)                            \
        )
#else
#    define TYPE_ID_POINTERS_INTPTR_TYPES(x) TYPE_ID_POINTERS_INT8_TYPES(x)
#endif

#define TYPE_ID_POINTERS_INT_TYPES(x)                                          \
    _Generic(                                                                  \
        (typeof_unqual(x)){},                                                  \
        TYPE_ID_CVA_POINTERS(size_t, Size),                                    \
        default: _Generic(                                                     \
            (typeof_unqual(x)){},                                              \
            TYPE_ID_CVA_POINTERS(ptrdiff_t, Ptrdiff),                          \
            default: _Generic(                                                 \
                (typeof_unqual(x)){},                                          \
                TYPE_ID_CVA_POINTERS(max_align_t, Max_Align),                  \
                default: _Generic(                                             \
                    (typeof_unqual(x)){},                                      \
                    TYPE_ID_CVA_POINTERS(intmax_t, Intmax),                    \
                    TYPE_ID_CVA_POINTERS(uintmax_t, UIntmax),                  \
                    default: _Generic(                                         \
                        (typeof_unqual(x)){},                                  \
                        TYPE_ID_CVA_POINTERS(wchar_t, WChar),                  \
                        default: TYPE_ID_POINTERS_INTPTR_TYPES(x)              \
                    )                                                          \
                )                                                              \
            )                                                                  \
        )                                                                      \
    )

#define TYPE_ID_POINTERS(x)                                                    \
    _Generic(                                                                  \
        (typeof_unqual(x)){},                                                  \
        TYPE_ID_CVA_POINTERS(char, Char),                                      \
        TYPE_ID_CVA_POINTERS(unsigned char, UChar),                            \
        TYPE_ID_CVA_POINTERS(signed char, SChar),                              \
        TYPE_ID_CVA_POINTERS(short, Short),                                    \
        TYPE_ID_CVA_POINTERS(unsigned short, UShort),                          \
        TYPE_ID_CVA_POINTERS(int, Int),                                        \
        TYPE_ID_CVA_POINTERS(unsigned int, UInt),                              \
        TYPE_ID_CVA_POINTERS(long, Long),                                      \
        TYPE_ID_CVA_POINTERS(unsigned long, ULong),                            \
        TYPE_ID_CVA_POINTERS(long long, LLong),                                \
        TYPE_ID_CVA_POINTERS(unsigned long long, ULLong),                      \
        TYPE_ID_CVA_POINTERS(float, Float),                                    \
        TYPE_ID_CVA_POINTERS(double, Double),                                  \
        TYPE_ID_CVA_POINTERS(long double, LDouble),                            \
        TYPE_ID_CVA_POINTERS(nullptr_t, Nullptr),                              \
        void *: TIDP(Void),                                                    \
        const void *: TIDP(Void),                                              \
        volatile void *: TIDP(Void),                                           \
        const volatile void *: TIDP(Void),                                     \
        default: TYPE_ID_POINTERS_INT_TYPES(x)                                 \
    )

#endif
