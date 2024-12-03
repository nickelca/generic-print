#ifndef TYPE_ID_COMMON_H
#define TYPE_ID_COMMON_H

#include <limits.h>
#include <stddef.h>
#include <stdint.h>

#define TIDI(ident) TID_##ident
#define TIDP(ident) TID_P_##ident
#define TID(ident) TIDI(ident), TIDP(ident)

#define TID_STRINGIZE(x) TID_STRINGIZE_1(x)
#define TID_STRINGIZE_1(x) #x

#define TIDI_NAME(ident) TID_STRINGIZE(TIDI(ident))
#define TIDP_NAME(ident) TID_STRINGIZE(TIDP(ident))
#define TID_NAME(ident) TIDI_NAME(ident), TIDP_NAME(ident)

enum Type_ID : int {
    /* guaranteed to be promoted */
    TIDP(Char),
    TIDP(UChar),
    TIDP(SChar),
    TIDP(Short),
    TIDP(UShort),
    TIDP(Bool),

    TID(Int),
    TID(UInt),
    TID(Long),
    TID(ULong),
    TID(LLong),
    TID(ULLong),

#if !((INT_MIN <= 0 && SIZE_MAX <= INT_MAX) || (SIZE_MAX <= UINT_MAX))
    TID(Size),
#else
    TIDP(Size),
#endif

#if !(                                                                         \
    (INT_MIN <= PTRDIFF_MIN && PTRDIFF_MAX <= INT_MAX) ||                      \
    (0 <= PTRDIFF_MIN && PTRDIFF_MAX <= UINT_MAX)                              \
)
    TID(Ptrdiff),
#else
    TIDP(Ptrdiff),
#endif

#if !(                                                                         \
    (INT_MIN <= INTMAX_MIN && INTMAX_MAX <= INT_MAX) ||                        \
    (0 <= INTMAX_MIN && INTMAX_MAX <= UINT_MAX)                                \
)
    TID(Intmax),
#else
    TIDP(Intmax),
#endif

#if !((INT_MIN <= 0 && UINTMAX_MAX <= INT_MAX) || (UINTMAX_MAX <= UINT_MAX))
    TID(UIntmax),
#else
    TIDP(UIntmax),
#endif

#if !(                                                                         \
    (INT_MIN <= INT_FAST8_MIN && INT_FAST8_MAX <= INT_MAX) ||                  \
    (0 <= INT_FAST8_MIN && INT_FAST8_MAX <= UINT_MAX)                          \
)
    TID(Int_Fast8),
#else
    TIDP(Int_Fast8),
#endif
#if !(                                                                         \
    (INT_MIN <= 0 && UINT_FAST8_MAX <= INT_MAX) ||                             \
    (UINT_FAST8_MAX <= UINT_MAX)                                               \
)
    TID(UInt_Fast8),
#else
    TIDP(UInt_Fast8),
#endif

#if !(                                                                         \
    (INT_MIN <= INT_FAST16_MIN && INT_FAST16_MAX <= INT_MAX) ||                \
    (0 <= INT_FAST16_MIN && INT_FAST16_MAX <= UINT_MAX)                        \
)
    TID(Int_Fast16),
#else
    TIDP(Int_Fast16),
#endif
#if !(                                                                         \
    (INT_MIN <= 0 && UINT_FAST16_MAX <= INT_MAX) ||                            \
    (UINT_FAST16_MAX <= UINT_MAX)                                              \
)
    TID(UInt_Fast16),
#else
    TIDP(UInt_Fast16),
#endif

#if !(                                                                         \
    (INT_MIN <= INT_FAST32_MIN && INT_FAST32_MAX <= INT_MAX) ||                \
    (0 <= INT_FAST32_MIN && INT_FAST32_MAX <= UINT_MAX)                        \
)
    TID(Int_Fast32),
#else
    TIDP(Int_Fast32),
#endif
#if !(                                                                         \
    (INT_MIN <= 0 && UINT_FAST32_MAX <= INT_MAX) ||                            \
    (UINT_FAST32_MAX <= UINT_MAX)                                              \
)
    TID(UInt_Fast32),
#else
    TIDP(UInt_Fast32),
#endif

#if !(                                                                         \
    (INT_MIN <= INT_FAST64_MIN && INT_FAST64_MAX <= INT_MAX) ||                \
    (0 <= INT_FAST64_MIN && INT_FAST64_MAX <= UINT_MAX)                        \
)
    TID(Int_Fast64),
#else
    TIDP(Int_Fast64),
#endif
#if !(                                                                         \
    (INT_MIN <= 0 && UINT_FAST64_MAX <= INT_MAX) ||                            \
    (UINT_FAST64_MAX <= UINT_MAX)                                              \
)
    TID(UInt_Fast64),
#else
    TIDP(UInt_Fast64),
#endif

#if !(                                                                         \
    (INT_MIN <= INT_LEAST8_MIN && INT_LEAST8_MAX <= INT_MAX) ||                \
    (0 <= INT_LEAST8_MIN && INT_LEAST8_MAX <= UINT_MAX)                        \
)
    TID(Int_Least8),
#else
    TIDP(Int_Least8),
#endif
#if !(                                                                         \
    (INT_MIN <= 0 && UINT_LEAST8_MAX <= INT_MAX) ||                            \
    (UINT_LEAST8_MAX <= UINT_MAX)                                              \
)
    TID(UInt_Least8),
#else
    TIDP(UInt_Least8),
#endif

#if !(                                                                         \
    (INT_MIN <= INT_LEAST16_MIN && INT_LEAST16_MAX <= INT_MAX) ||              \
    (0 <= INT_LEAST16_MIN && INT_LEAST16_MAX <= UINT_MAX)                      \
)
    TID(Int_Least16),
#else
    TIDP(Int_Least16),
#endif
#if !(                                                                         \
    (INT_MIN <= 0 && UINT_LEAST16_MAX <= INT_MAX) ||                           \
    (UINT_LEAST16_MAX <= UINT_MAX)                                             \
)
    TID(UInt_Least16),
#else
    TIDP(UInt_Least16),
#endif

#if !(                                                                         \
    (INT_MIN <= INT_LEAST32_MIN && INT_LEAST32_MAX <= INT_MAX) ||              \
    (0 <= INT_LEAST32_MIN && INT_LEAST32_MAX <= UINT_MAX)                      \
)
    TID(Int_Least32),
#else
    TIDP(Int_Least32),
#endif
#if !(                                                                         \
    (INT_MIN <= 0 && UINT_LEAST32_MAX <= INT_MAX) ||                           \
    (UINT_LEAST32_MAX <= UINT_MAX)                                             \
)
    TID(UInt_Least32),
#else
    TIDP(UInt_Least32),
#endif

#if !(                                                                         \
    (INT_MIN <= INT_LEAST64_MIN && INT_LEAST64_MAX <= INT_MAX) ||              \
    (0 <= INT_LEAST64_MIN && INT_LEAST64_MAX <= UINT_MAX)                      \
)
    TID(Int_Least64),
#else
    TIDP(Int_Least64),
#endif
#if !(                                                                         \
    (INT_MIN <= 0 && UINT_LEAST64_MAX <= INT_MAX) ||                           \
    (UINT_LEAST64_MAX <= UINT_MAX)                                             \
)
    TID(UInt_Least64),
#else
    TIDP(UInt_Least64),
#endif

#ifdef INTPTR_MAX
    TID(Intptr),
#endif
#ifdef UINTPTR_MAX
    TID(UIntptr),
#endif
#ifdef INT8_MAX
    TID(Int8),
#endif
#ifdef UINT8_MAX
    TID(UInt8),
#endif
#ifdef INT16_MAX
    TID(Int16),
#endif
#ifdef UINT16_MAX
    TID(UInt16),
#endif
#ifdef INT32_MAX
    TID(Int32),
#endif
#ifdef UINT32_MAX
    TID(UInt32),
#endif
#ifdef INT64_MAX
    TID(Int64),
#endif
#ifdef UINT64_MAX
    TID(UInt64),
#endif

#if !(                                                                         \
    (INT_MIN <= WCHAR_MIN && WCHAR_MAX <= INT_MAX) ||                          \
    (0 <= WCHAR_MIN && WCHAR_MAX <= UINT_MAX)                                  \
)
    TID(WChar),
#else
    TIDP(WChar),
#endif

    /* guaranteed to be promoted */
    TIDP(Float),

    TID(Double),
    TID(LDouble),

    /* TODO: What to do with this */
    TID(Max_Align),

    TID(Nullptr),

    TIDP(Void),
    TIDI(Invalid),
    TIDI(Sentinel),
};

#if INT_MIN <= WCHAR_MIN && WCHAR_MAX <= INT_MAX
#    define TID_Alias_WChar
#    define TID_WChar TID_Int
#elif 0 <= WCHAR_MIN && WCHAR_MAX <= UINT_MAX
#    define TID_Alias_WChar
#    define TID_WChar TID_UInt
#endif

#if INT_MIN <= 0 && SIZE_MAX <= INT_MAX
#    define TID_Alias_Size
#    define TID_Size TID_Int
#elif SIZE_MAX <= UINT_MAX
#    define TID_Alias_Size
#    define TID_Size TID_UInt
#endif

#if INT_MIN <= PTRDIFF_MIN && PTRDIFF_MAX <= INT_MAX
#    define TID_Alias_Ptrdiff
#    define TID_Ptrdiff TID_Int
#elif 0 <= PTRDIFF_MIN && PTRDIFF_MAX <= UINT_MAX
#    define TID_Alias_Ptrdiff
#    define TID_Ptrdiff TID_UInt
#endif

#if INT_MIN <= INTMAX_MIN && INTMAX_MAX <= INT_MAX
#    define TID_Alias_Intmax
#    define TID_Intmax TID_Int
#elif 0 <= INTMAX_MIN && INTMAX_MAX <= UINT_MAX
#    define TID_Alias_Intmax
#    define TID_Intmax TID_UInt
#endif

#if INT_MIN <= 0 && UINTMAX_MAX <= INT_MAX
#    define TID_Alias_UIntmax
#    define TID_UIntmax TID_Int
#elif UINTMAX_MAX <= UINT_MAX
#    define TID_Alias_UIntmax
#    define TID_UIntmax TID_UInt
#endif

#if INT_MIN <= INT_FAST8_MIN && INT_FAST8_MAX <= INT_MAX
#    define TID_Alias_Int_Fast8
#    define TID_Int_Fast8 TID_Int
#elif 0 <= INT_FAST8_MIN && INT_FAST8_MAX <= UINT_MAX
#    define TID_Alias_Int_Fast8
#    define TID_Int_Fast8 TID_UInt
#endif
#if INT_MIN <= 0 && UINT_FAST8_MAX <= INT_MAX
#    define TID_Alias_UInt_Fast8
#    define TID_UInt_Fast8 TID_Int
#elif UINT_FAST8_MAX <= UINT_MAX
#    define TID_Alias_UInt_Fast8
#    define TID_UInt_Fast8 TID_UInt
#endif

#if INT_MIN <= INT_FAST16_MIN && INT_FAST16_MAX <= INT_MAX
#    define TID_Alias_Int_Fast16
#    define TID_Int_Fast16 TID_Int
#elif 0 <= INT_FAST16_MIN && INT_FAST16_MAX <= UINT_MAX
#    define TID_Alias_Int_Fast16
#    define TID_Int_Fast16 TID_UInt
#endif
#if INT_MIN <= 0 && UINT_FAST16_MAX <= INT_MAX
#    define TID_Alias_UInt_Fast16
#    define TID_UInt_Fast16 TID_Int
#elif UINT_FAST16_MAX <= UINT_MAX
#    define TID_Alias_UInt_Fast16
#    define TID_UInt_Fast16 TID_UInt
#endif

#if INT_MIN <= INT_FAST32_MIN && INT_FAST32_MAX <= INT_MAX
#    define TID_Alias_Int_Fast32
#    define TID_Int_Fast32 TID_Int
#elif 0 <= INT_FAST32_MIN && INT_FAST32_MAX <= UINT_MAX
#    define TID_Alias_Int_Fast32
#    define TID_Int_Fast32 TID_UInt
#endif
#if INT_MIN <= 0 && UINT_FAST32_MAX <= INT_MAX
#    define TID_Alias_UInt_Fast32
#    define TID_UInt_Fast32 TID_Int
#elif UINT_FAST32_MAX <= UINT_MAX
#    define TID_Alias_UInt_Fast32
#    define TID_UInt_Fast32 TID_UInt
#endif

#if INT_MIN <= INT_FAST64_MIN && INT_FAST64_MAX <= INT_MAX
#    define TID_Alias_Int_Fast64
#    define TID_Int_Fast64 TID_Int
#elif 0 <= INT_FAST64_MIN && INT_FAST64_MAX <= UINT_MAX
#    define TID_Alias_Int_Fast64
#    define TID_Int_Fast64 TID_UInt
#endif
#if INT_MIN <= 0 && UINT_FAST64_MAX <= INT_MAX
#    define TID_Alias_UInt_Fast64
#    define TID_UInt_Fast64 TID_Int
#elif UINT_FAST64_MAX <= UINT_MAX
#    define TID_Alias_UInt_Fast64
#    define TID_UInt_Fast64 TID_UInt
#endif

#if INT_MIN <= INT_LEAST8_MIN && INT_LEAST8_MAX <= INT_MAX
#    define TID_Alias_Int_Least8
#    define TID_Int_Least8 TID_Int
#elif 0 <= INT_LEAST8_MIN && INT_LEAST8_MAX <= UINT_MAX
#    define TID_Alias_Int_Least8
#    define TID_Int_Least8 TID_UInt
#endif
#if INT_MIN <= 0 && UINT_LEAST8_MAX <= INT_MAX
#    define TID_Alias_UInt_Least8
#    define TID_UInt_Least8 TID_Int
#elif UINT_LEAST8_MAX <= UINT_MAX
#    define TID_Alias_UInt_Least8
#    define TID_UInt_Least8 TID_UInt
#endif

#if INT_MIN <= INT_LEAST16_MIN && INT_LEAST16_MAX <= INT_MAX
#    define TID_Alias_Int_Least16
#    define TID_Int_Least16 TID_Int
#elif 0 <= INT_LEAST16_MIN && INT_LEAST16_MAX <= UINT_MAX
#    define TID_Alias_Int_Least16
#    define TID_Int_Least16 TID_UInt
#endif
#if INT_MIN <= 0 && UINT_LEAST16_MAX <= INT_MAX
#    define TID_Alias_UInt_Least16
#    define TID_UInt_Least16 TID_Int
#elif UINT_LEAST16_MAX <= UINT_MAX
#    define TID_Alias_UInt_Least16
#    define TID_UInt_Least16 TID_UInt
#endif

#if INT_MIN <= INT_LEAST32_MIN && INT_LEAST32_MAX <= INT_MAX
#    define TID_Alias_Int_Least32
#    define TID_Int_Least32 TID_Int
#elif 0 <= INT_LEAST32_MIN && INT_LEAST32_MAX <= UINT_MAX
#    define TID_Alias_Int_Least32
#    define TID_Int_Least32 TID_UInt
#endif
#if INT_MIN <= 0 && UINT_LEAST32_MAX <= INT_MAX
#    define TID_Alias_UInt_Least32
#    define TID_UInt_Least32 TID_Int
#elif UINT_LEAST32_MAX <= UINT_MAX
#    define TID_Alias_UInt_Least32
#    define TID_UInt_Least32 TID_UInt
#endif

#if INT_MIN <= INT_LEAST64_MIN && INT_LEAST64_MAX <= INT_MAX
#    define TID_Alias_Int_Least64
#    define TID_Int_Least64 TID_Int
#elif 0 <= INT_LEAST64_MIN && INT_LEAST64_MAX <= UINT_MAX
#    define TID_Alias_Int_Least64
#    define TID_Int_Least64 TID_UInt
#endif
#if INT_MIN <= 0 && UINT_LEAST64_MAX <= INT_MAX
#    define TID_Alias_UInt_Least64
#    define TID_UInt_Least64 TID_Int
#elif UINT_LEAST64_MAX <= UINT_MAX
#    define TID_Alias_UInt_Least64
#    define TID_UInt_Least64 TID_UInt
#endif

#if defined(INTPTR_MAX) && INT_MIN <= INTPTR_MIN && INTPTR_MAX <= INT_MAX
#    define TID_Alias_Intptr
#    define TID_Intptr TID_Int
#elif defined(INTPTR_MAX) && 0 <= INTPTR_MIN && INTPTR_MAX <= UINT_MAX
#    define TID_Alias_Intptr
#    define TID_Intptr TID_UInt
#endif
#if defined(UINTPTR_MAX) && INT_MIN <= 0 && UINTPTR_MAX <= INT_MAX
#    define TID_Alias_UIntptr
#    define TID_UIntptr TID_Int
#elif defined(UINTPTR_MAX) && UINTPTR_MAX <= UINT_MAX
#    define TID_Alias_UIntptr
#    define TID_UIntptr TID_UInt
#endif

#if defined(INT8_MAX) && INT_MIN <= INT8_MIN && INT8_MAX <= INT_MAX
#    define TID_Alias_Int8
#    define TID_Int8 TID_Int
#elif defined(INT8_MAX) && 0 <= INT8_MIN && INT8_MAX <= UINT_MAX
#    define TID_Alias_Int8
#    define TID_Int8 TID_UInt
#endif
#if defined(UINT8_MAX) && INT_MIN <= 0 && UINT8_MAX <= INT_MAX
#    define TID_Alias_UInt8
#    define TID_UInt8 TID_Int
#elif defined(UINT8_MAX) && UINT8_MAX <= UINT_MAX
#    define TID_Alias_UInt8
#    define TID_UInt8 TID_UInt
#endif

#if defined(INT16_MAX) && INT_MIN <= INT16_MIN && INT16_MAX <= INT_MAX
#    define TID_Alias_Int16
#    define TID_Int16 TID_Int
#elif defined(INT16_MAX) && 0 <= INT16_MIN && INT16_MAX <= UINT_MAX
#    define TID_Alias_Int16
#    define TID_Int16 TID_UInt
#endif
#if defined(UINT16_MAX) && INT_MIN <= 0 && UINT16_MAX <= INT_MAX
#    define TID_Alias_UInt16
#    define TID_UInt16 TID_Int
#elif defined(UINT16_MAX) && UINT16_MAX <= UINT_MAX
#    define TID_Alias_UInt16
#    define TID_UInt16 TID_UInt
#endif

#if defined(INT32_MAX) && INT_MIN <= INT32_MIN && INT32_MAX <= INT_MAX
#    define TID_Alias_Int32
#    define TID_Int32 TID_Int
#elif defined(INT32_MAX) && 0 <= INT32_MIN && INT32_MAX <= UINT_MAX
#    define TID_Alias_Int32
#    define TID_Int32 TID_UInt
#endif
#if defined(UINT32_MAX) && INT_MIN <= 0 && UINT32_MAX <= INT_MAX
#    define TID_Alias_UInt32
#    define TID_UInt32 TID_Int
#elif defined(UINT32_MAX) && UINT32_MAX <= UINT_MAX
#    define TID_Alias_UInt32
#    define TID_UInt32 TID_UInt
#endif

#if defined(INT64_MAX) && INT_MIN <= INT64_MIN && INT64_MAX <= INT_MAX
#    define TID_Alias_Int64
#    define TID_Int64 TID_Int
#elif defined(INT64_MAX) && 0 <= INT64_MIN && INT64_MAX <= UINT_MAX
#    define TID_Alias_Int64
#    define TID_Int64 TID_UInt
#endif
#if defined(UINT64_MAX) && INT_MIN <= 0 && UINT64_MAX <= INT_MAX
#    define TID_Alias_UInt64
#    define TID_UInt64 TID_Int
#elif defined(UINT64_MAX) && UINT64_MAX <= UINT_MAX
#    define TID_Alias_UInt64
#    define TID_UInt64 TID_UInt
#endif

#define TID_Alias_Char
#if INT_MIN <= CHAR_MIN && CHAR_MAX <= INT_MAX
#    define TID_Char TID_Int
#else
#    define TID_Char TID_UInt
#endif

#define TID_Alias_UChar
#if INT_MIN <= 0 && UCHAR_MAX <= INT_MAX
#    define TID_UChar TID_Int
#else
#    define TID_UChar TID_UInt
#endif

#define TID_Alias_SChar
#if INT_MIN <= SCHAR_MIN && SCHAR_MAX <= INT_MAX
#    define TID_SChar TID_Int
#else
#    define TID_SChar TID_UInt
#endif

#define TID_Alias_Short
#if INT_MIN <= SHRT_MIN && SHRT_MAX <= INT_MAX
#    define TID_Short TID_Int
#else
#    define TID_Short TID_UInt
#endif

#define TID_Alias_UShort
#if INT_MIN <= 0 && USHRT_MAX <= INT_MAX
#    define TID_UShort TID_Int
#else
#    define TID_UShort TID_UInt
#endif

#define TID_Alias_Bool
#if INT_MIN <= 0 && BOOL_MAX <= INT_MAX
#    define TID_Bool TID_Int
#else
#    define TID_Bool TID_UInt
#endif

#define TID_Alias_Float
#define TID_Float TID_Double

#endif
