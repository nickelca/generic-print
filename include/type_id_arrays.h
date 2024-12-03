#ifndef TYPE_ID_ARRAYS_H
#define TYPE_ID_ARRAYS_H

#include <stddef.h>
#include <stdint.h>
#include <wchar.h>

#include "type_id_common.h"

// clang-format off
#define TYPE_ID_CVA_ARRAYS(T, ID)                                              \
    T(*)[]: TIDP(ID),                                                          \
    const T(*)[]: TIDP(ID),                                                    \
    volatile T(*)[]: TIDP(ID),                                                 \
    const volatile T(*)[]: TIDP(ID),                                           \
    _Atomic T(*)[]: TIDP(ID),                                                  \
    const _Atomic T(*)[]: TIDP(ID),                                            \
    const _Atomic volatile T(*)[]: TIDP(ID)
// clang-format on

#define TYPE_ID_ARRAYS_INTN_LEAST_TYPES(x)                                     \
    _Generic(                                                                  \
        &(typeof_unqual(x)){},                                                 \
        TYPE_ID_CVA_ARRAYS(int_least8_t, Int_Least8),                          \
        TYPE_ID_CVA_ARRAYS(uint_least8_t, UInt_Least8),                        \
        default: _Generic(                                                     \
            &(typeof_unqual(x)){},                                             \
            TYPE_ID_CVA_ARRAYS(int_least16_t, Int_Least16),                    \
            TYPE_ID_CVA_ARRAYS(uint_least16_t, UInt_Least16),                  \
            default: _Generic(                                                 \
                &(typeof_unqual(x)){},                                         \
                TYPE_ID_CVA_ARRAYS(int_least32_t, Int_Least32),                \
                TYPE_ID_CVA_ARRAYS(uint_least32_t, UInt_Least32),              \
                default: _Generic(                                             \
                    &(typeof_unqual(x)){},                                     \
                    TYPE_ID_CVA_ARRAYS(int_least64_t, Int_Least64),            \
                    TYPE_ID_CVA_ARRAYS(uint_least64_t, UInt_Least64),          \
                    default: TYPE_ID_ARRAYS_DEFAULT(x)                         \
                )                                                              \
            )                                                                  \
        )                                                                      \
    )

#define TYPE_ID_ARRAYS_INTN_FAST_TYPES(x)                                      \
    _Generic(                                                                  \
        &(typeof_unqual(x)){},                                                 \
        TYPE_ID_CVA_ARRAYS(int_fast8_t, Int_Fast8),                            \
        TYPE_ID_CVA_ARRAYS(uint_fast8_t, UInt_Fast8),                          \
        default: _Generic(                                                     \
            &(typeof_unqual(x)){},                                             \
            TYPE_ID_CVA_ARRAYS(int_fast16_t, Int_Fast16),                      \
            TYPE_ID_CVA_ARRAYS(uint_fast16_t, UInt_Fast16),                    \
            default: _Generic(                                                 \
                &(typeof_unqual(x)){},                                         \
                TYPE_ID_CVA_ARRAYS(int_fast32_t, Int_Fast32),                  \
                TYPE_ID_CVA_ARRAYS(uint_fast32_t, UInt_Fast32),                \
                default: _Generic(                                             \
                    &(typeof_unqual(x)){},                                     \
                    TYPE_ID_CVA_ARRAYS(int_fast64_t, Int_Fast64),              \
                    TYPE_ID_CVA_ARRAYS(uint_fast64_t, UInt_Fast64),            \
                    default: TYPE_ID_ARRAYS_INTN_LEAST_TYPES(x)                \
                )                                                              \
            )                                                                  \
        )                                                                      \
    )

#ifdef INT64_MAX
#    define TYPE_ID_ARRAYS_INT64_TYPES(x)                                      \
        _Generic(                                                              \
            &(typeof_unqual(x)){},                                             \
            TYPE_ID_CVA_ARRAYS(int64_t, Int64),                                \
            TYPE_ID_CVA_ARRAYS(uint64_t, UInt64),                              \
            default: TYPE_ID_ARRAYS_INTN_FAST_TYPES(x)                         \
        )
#else
#    define TYPE_ID_ARRAYS_INT64_TYPES(x) TYPE_ID_ARRAYS_INTN_FAST_TYPES(x)
#endif

#ifdef INT32_MAX
#    define TYPE_ID_ARRAYS_INT32_TYPES(x)                                      \
        _Generic(                                                              \
            &(typeof_unqual(x)){},                                             \
            TYPE_ID_CVA_ARRAYS(int32_t, Int32),                                \
            TYPE_ID_CVA_ARRAYS(uint32_t, UInt32),                              \
            default: TYPE_ID_ARRAYS_INT64_TYPES(x)                             \
        )
#else
#    define TYPE_ID_ARRAYS_INT32_TYPES(x) TYPE_ID_ARRAYS_INT64_TYPES(x)
#endif

#ifdef INT16_MAX
#    define TYPE_ID_ARRAYS_INT16_TYPES(x)                                      \
        _Generic(                                                              \
            &(typeof_unqual(x)){},                                             \
            TYPE_ID_CVA_ARRAYS(int16_t, Int16),                                \
            TYPE_ID_CVA_ARRAYS(uint16_t, UInt16),                              \
            default: TYPE_ID_ARRAYS_INT32_TYPES(x)                             \
        )
#else
#    define TYPE_ID_ARRAYS_INT16_TYPES(x) TYPE_ID_ARRAYS_INT32_TYPES(x)
#endif

#ifdef INT8_MAX
#    define TYPE_ID_ARRAYS_INT8_TYPES(x)                                       \
        _Generic(                                                              \
            &(typeof_unqual(x)){},                                             \
            TYPE_ID_CVA_ARRAYS(int8_t, Int8),                                  \
            TYPE_ID_CVA_ARRAYS(uint8_t, UInt8),                                \
            default: TYPE_ID_ARRAYS_INT16_TYPES(x)                             \
        )
#else
#    define TYPE_ID_ARRAYS_INT8_TYPES(x) TYPE_ID_ARRAYS_INT16_TYPES(x)
#endif

#ifdef INTPTR_MAX
#    define TYPE_ID_ARRAYS_INTPTR_TYPES(x)                                     \
        _Generic(                                                              \
            &(typeof_unqual(x)){},                                             \
            TYPE_ID_CVA_ARRAYS(intptr_t, Intptr),                              \
            TYPE_ID_CVA_ARRAYS(uintptr_t, UIntptr),                            \
            default: TYPE_ID_ARRAYS_INT8_TYPES(x)                              \
        )
#else
#    define TYPE_ID_ARRAYS_INTPTR_TYPES(x) TYPE_ID_ARRAYS_INT8_TYPES(x)
#endif

#define TYPE_ID_ARRAYS_INT_TYPES(x)                                            \
    _Generic(                                                                  \
        &(typeof_unqual(x)){},                                                 \
        TYPE_ID_CVA_ARRAYS(size_t, Size),                                      \
        default: _Generic(                                                     \
            &(typeof_unqual(x)){},                                             \
            TYPE_ID_CVA_ARRAYS(ptrdiff_t, Ptrdiff),                            \
            default: _Generic(                                                 \
                &(typeof_unqual(x)){},                                         \
                TYPE_ID_CVA_ARRAYS(max_align_t, Max_Align),                    \
                default: _Generic(                                             \
                    &(typeof_unqual(x)){},                                     \
                    TYPE_ID_CVA_ARRAYS(intmax_t, Intmax),                      \
                    TYPE_ID_CVA_ARRAYS(uintmax_t, UIntmax),                    \
                    default: _Generic(                                         \
                        &(typeof_unqual(x)){},                                 \
                        TYPE_ID_CVA_ARRAYS(wchar_t, WChar),                    \
                        default: TYPE_ID_ARRAYS_INTPTR_TYPES(x)                \
                    )                                                          \
                )                                                              \
            )                                                                  \
        )                                                                      \
    )

#define TYPE_ID_ARRAYS(x)                                                      \
    _Generic(                                                                  \
        &(typeof_unqual(x)){},                                                 \
        TYPE_ID_CVA_ARRAYS(char, Char),                                        \
        TYPE_ID_CVA_ARRAYS(unsigned char, UChar),                              \
        TYPE_ID_CVA_ARRAYS(signed char, SChar),                                \
        TYPE_ID_CVA_ARRAYS(short, Short),                                      \
        TYPE_ID_CVA_ARRAYS(unsigned short, UShort),                            \
        TYPE_ID_CVA_ARRAYS(int, Int),                                          \
        TYPE_ID_CVA_ARRAYS(unsigned int, UInt),                                \
        TYPE_ID_CVA_ARRAYS(long, Long),                                        \
        TYPE_ID_CVA_ARRAYS(unsigned long, ULong),                              \
        TYPE_ID_CVA_ARRAYS(long long, LLong),                                  \
        TYPE_ID_CVA_ARRAYS(unsigned long long, ULLong),                        \
        TYPE_ID_CVA_ARRAYS(float, Float),                                      \
        TYPE_ID_CVA_ARRAYS(double, Double),                                    \
        TYPE_ID_CVA_ARRAYS(long double, LDouble),                              \
        TYPE_ID_CVA_ARRAYS(nullptr_t, Nullptr),                                \
        default: TYPE_ID_ARRAYS_INT_TYPES(x)                                   \
    )

#endif
