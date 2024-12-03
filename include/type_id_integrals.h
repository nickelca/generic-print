#ifndef TYPE_ID_INTEGRALS_H
#define TYPE_ID_INTEGRALS_H

#include <stddef.h>
#include <stdint.h>
#include <wchar.h>

#include "type_id_common.h"

#define TYPE_ID_INTEGRALS_INTN_LEAST_TYPES(x)                                  \
    _Generic(                                                                  \
        (typeof_unqual(x)){},                                                  \
        int_least8_t: TIDI(Int_Least8),                                        \
        uint_least8_t: TIDI(UInt_Least8),                                      \
        default: _Generic(                                                     \
            (typeof_unqual(x)){},                                              \
            int_least16_t: TIDI(Int_Least16),                                  \
            uint_least16_t: TIDI(UInt_Least16),                                \
            default: _Generic(                                                 \
                (typeof_unqual(x)){},                                          \
                int_least32_t: TIDI(Int_Least32),                              \
                uint_least32_t: TIDI(UInt_Least32),                            \
                default: _Generic(                                             \
                    (typeof_unqual(x)){},                                      \
                    int_least64_t: TIDI(Int_Least64),                          \
                    uint_least64_t: TIDI(UInt_Least64),                        \
                    default: TYPE_ID_INTEGRALS_DEFAULT(x)                      \
                )                                                              \
            )                                                                  \
        )                                                                      \
    )

#define TYPE_ID_INTEGRALS_INTN_FAST_TYPES(x)                                   \
    _Generic(                                                                  \
        (typeof_unqual(x)){},                                                  \
        int_fast8_t: TIDI(Int_Fast8),                                          \
        uint_fast8_t: TIDI(UInt_Fast8),                                        \
        default: _Generic(                                                     \
            (typeof_unqual(x)){},                                              \
            int_fast16_t: TIDI(Int_Fast16),                                    \
            uint_fast16_t: TIDI(UInt_Fast16),                                  \
            default: _Generic(                                                 \
                (typeof_unqual(x)){},                                          \
                int_fast32_t: TIDI(Int_Fast32),                                \
                uint_fast32_t: TIDI(UInt_Fast32),                              \
                default: _Generic(                                             \
                    (typeof_unqual(x)){},                                      \
                    int_fast64_t: TIDI(Int_Fast64),                            \
                    uint_fast64_t: TIDI(UInt_Fast64),                          \
                    default: TYPE_ID_INTEGRALS_INTN_LEAST_TYPES(x)             \
                )                                                              \
            )                                                                  \
        )                                                                      \
    )

#ifdef INT64_MAX
#    define TYPE_ID_INTEGRALS_INT64_TYPES(x)                                   \
        _Generic(                                                              \
            (typeof_unqual(x)){},                                              \
            int64_t: TIDI(Int64),                                              \
            uint64_t: TIDI(UInt64),                                            \
            default: TYPE_ID_INTEGRALS_INTN_FAST_TYPES(x)                      \
        )
#else
#    define TYPE_ID_INTEGRALS_INT64_TYPES(x)                                   \
        TYPE_ID_INTEGRALS_INTN_FAST_TYPES(x)
#endif

#ifdef INT32_MAX
#    define TYPE_ID_INTEGRALS_INT32_TYPES(x)                                   \
        _Generic(                                                              \
            (typeof_unqual(x)){},                                              \
            int32_t: TIDI(Int32),                                              \
            uint32_t: TIDI(UInt32),                                            \
            default: TYPE_ID_INTEGRALS_INT64_TYPES(x)                          \
        )
#else
#    define TYPE_ID_INTEGRALS_INT32_TYPES(x) TYPE_ID_INTEGRALS_INT64_TYPES(x)
#endif

#ifdef INT16_MAX
#    define TYPE_ID_INTEGRALS_INT16_TYPES(x)                                   \
        _Generic(                                                              \
            (typeof_unqual(x)){},                                              \
            int16_t: TIDI(Int16),                                              \
            uint16_t: TIDI(UInt16),                                            \
            default: TYPE_ID_INTEGRALS_INT32_TYPES(x)                          \
        )
#else
#    define TYPE_ID_INTEGRALS_INT16_TYPES(x) TYPE_ID_INTEGRALS_INT32_TYPES(x)
#endif

#ifdef INT8_MAX
#    define TYPE_ID_INTEGRALS_INT8_TYPES(x)                                    \
        _Generic(                                                              \
            (typeof_unqual(x)){},                                              \
            int8_t: TIDI(Int8),                                                \
            uint8_t: TIDI(UInt8),                                              \
            default: TYPE_ID_INTEGRALS_INT16_TYPES(x)                          \
        )
#else
#    define TYPE_ID_INTEGRALS_INT8_TYPES(x) TYPE_ID_INTEGRALS_INT16_TYPES(x)
#endif

#ifdef INTPTR_MAX
#    define TYPE_ID_INTEGRALS_INTPTR_TYPES(x)                                  \
        _Generic(                                                              \
            (typeof_unqual(x)){},                                              \
            intptr_t: TIDI(Intptr),                                            \
            uintptr_t: TIDI(UIntptr),                                          \
            default: TYPE_ID_INTEGRALS_INT8_TYPES(x)                           \
        )
#else
#    define TYPE_ID_INTEGRALS_INTPTR_TYPES(x) TYPE_ID_INTEGRALS_INT8_TYPES(x)
#endif

#define TYPE_ID_INTEGRALS_INT_TYPES(x)                                         \
    _Generic(                                                                  \
        (typeof_unqual(x)){},                                                  \
        size_t: TIDI(Size),                                                    \
        default: _Generic(                                                     \
            (typeof_unqual(x)){},                                              \
            ptrdiff_t: TIDI(Ptrdiff),                                          \
            default: _Generic(                                                 \
                (typeof_unqual(x)){},                                          \
                max_align_t: TIDI(Max_Align),                                  \
                default: _Generic(                                             \
                    (typeof_unqual(x)){},                                      \
                    intmax_t: TIDI(Intmax),                                    \
                    uintmax_t: TIDI(UIntmax),                                  \
                    default: _Generic(                                         \
                        (typeof_unqual(x)){},                                  \
                        wchar_t: TIDI(WChar),                                  \
                        default: TYPE_ID_INTEGRALS_INTPTR_TYPES(x)             \
                    )                                                          \
                )                                                              \
            )                                                                  \
        )                                                                      \
    )

#define TYPE_ID_INTEGRALS(x)                                                   \
    _Generic(                                                                  \
        (typeof_unqual(x)){},                                                  \
        bool: TIDI(Bool),                                                      \
        char: TIDI(Char),                                                      \
        unsigned char: TIDI(UChar),                                            \
        signed char: TIDI(SChar),                                              \
        short: TIDI(Short),                                                    \
        unsigned short: TIDI(UShort),                                          \
                                                                               \
        int: TIDI(Int),                                                        \
        long: TIDI(Long),                                                      \
        long long: TIDI(LLong),                                                \
        unsigned int: TIDI(UInt),                                              \
        unsigned long: TIDI(ULong),                                            \
        unsigned long long: TIDI(ULLong),                                      \
                                                                               \
        float: TIDI(Float),                                                    \
        double: TIDI(Double),                                                  \
        long double: TIDI(LDouble),                                            \
        nullptr_t: TIDI(Nullptr),                                              \
                                                                               \
        default: TYPE_ID_INTEGRALS_INT_TYPES(x)                                \
    )

#endif
