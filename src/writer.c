#include "../include/writer.h"
#include "../include/type_id.h"
#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// clang-format off

static size_t
STD_Out_Write(void *ctx, const char *buf, size_t count)
{
    return fwrite(buf, 1, count, ctx);
}

struct Writer
Get_STD_Out(void)
{
    static struct Writer stdout_w = {};
    stdout_w.ctx = stdout;
    stdout_w.write_fn = STD_Out_Write;
    return stdout_w;
}

size_t
Write(struct Writer writer, const char *buf, size_t count)
{
    return writer.write_fn(writer.ctx, buf, count);
}

int
Write_All(struct Writer writer, const char *buf, size_t count)
{
    for (size_t i = 0; i < count;) {
        size_t written = Write(writer, buf, count);
        if (written < count) {
            return E_Writer_Short_Count;
        }
        i += written;
    }
    return 0;
}

struct Format_Placeholder {
    enum : char {
        format_specifier_default,
        format_specifier_binary,
        format_specifier_decimal,
        format_specifier_octal,
        format_specifier_hex,
        format_specifier_Hex,
        format_specifier_char,
        format_specifier_string,
    } specifier;
    char fill;
    enum : char {
        format_alignment_left,
        format_alignment_middle,
        format_alignment_right,
    } alignment;
    int width;
    int prec;
    enum : char {
        format_type_string,
        format_type_integer,
        format_type_float,
    } type;
};

size_t
strnlen(const char *s, size_t n)
{
    const char *end = memchr(s, 0, n);
    return s ? (size_t)(end - s) : n;
}

int
Format_Char_P(struct Writer writer, struct Format_Placeholder fmt, const char *str)
{
    size_t len = 0;
    if (fmt.width < 0) {
        len = strlen(str);
    } else {
        len = strnlen(str, fmt.width);
    }

    return Write_All(writer, str, len);
}

#define FORMAT_INTEGER_COMMON(U_width)                                         \
    unsigned base2 = base * base;                                              \
    char *buf = malloc(buf_len);                                               \
    if (!buf) {                                                                \
        return E_Writer_Malloc;                                                \
    }                                                                          \
    size_t i = buf_len;                                                        \
    while (n >= base2) {                                                       \
        unsigned _BitInt(U_width) d = n % base2;                               \
        i -= 2;                                                                \
        memcpy(&buf[i], &digits2[2 * d], 2);                                   \
        n /= base2;                                                            \
    }                                                                          \
    if (n < base) {                                                            \
        i -= 1;                                                                \
        buf[i] = digits1[n];                                                   \
    } else {                                                                   \
        i -= 2;                                                                \
        memcpy(&buf[i], &digits2[2 * n], 2);                                   \
    }                                                                          \
    if (base == 2) {                                                           \
        i -= 2;                                                                \
        memcpy(&buf[i], "0b", 2);                                              \
    } else if (base == 8) {                                                    \
        i -= 2;                                                                \
        memcpy(&buf[i], "0o", 2);                                              \
    } else if (base == 16) {                                                   \
        i -= 2;                                                                \
        memcpy(&buf[i], "0x", 2);                                              \
    }

#define FORMAT_INTEGER_DECL(ID, T)                                             \
    int Format_Integer_##ID(                                                   \
        struct Writer writer,                                                  \
        T x,                                                                   \
        unsigned int base,                                                     \
        const char *digits1,                                                   \
        const char *digits2                                                    \
    )

#define FORMAT_SIGNED_INTEGER_DEFN(ID, S, S_min, S_max, S_width)               \
    FORMAT_INTEGER_DECL(ID, S)                                                 \
    {                                                                          \
        int ret;                                                               \
        unsigned _BitInt(S_width) n;                                           \
        if (x == S_min) {                                                      \
            n = (unsigned _BitInt(S_width))(S_max) + (unsigned _BitInt(S_width))1;\
        } else if (x < 0) {                                                    \
            n = -x;                                                            \
        } else {                                                               \
            n = x;                                                             \
        }                                                                      \
        size_t buf_len = CHAR_BIT * S_width                                    \
            + 2 /* prefix */                                                   \
            + 1 /* negative sign */;                                           \
        FORMAT_INTEGER_COMMON(S_width)                                         \
        if (x < 0) {                                                           \
            i -= 1;                                                            \
            buf[i] = '-';                                                      \
        }                                                                      \
        ret = Write_All(writer, &buf[i], buf_len - i);                         \
        if (ret != 0) {                                                        \
            return ret;                                                        \
        }                                                                      \
        free(buf);                                                             \
        return 0;                                                              \
    }

#define FORMAT_UNSIGNED_INTEGER_DEFN(ID, U, U_max, U_width)                    \
    FORMAT_INTEGER_DECL(ID, U)                                                 \
    {                                                                          \
        int ret;                                                               \
        auto n = x;                                                            \
        size_t buf_len = CHAR_BIT * U_width                                    \
            + 2 /* prefix */;                                                  \
        FORMAT_INTEGER_COMMON(U_width)                                         \
        ret = Write_All(writer, &buf[i], buf_len - i);                         \
        if (ret != 0) {                                                        \
            return ret;                                                        \
        }                                                                      \
        free(buf);                                                             \
        return 0;                                                              \
    }

FORMAT_SIGNED_INTEGER_DEFN(Int, int, INT_MIN, INT_MAX, INT_WIDTH)
FORMAT_UNSIGNED_INTEGER_DEFN(UInt, unsigned int, UINT_MAX, UINT_WIDTH)
FORMAT_SIGNED_INTEGER_DEFN(Long, long, LONG_MIN, LONG_MAX, LONG_WIDTH)
FORMAT_UNSIGNED_INTEGER_DEFN(ULong, unsigned long, ULONG_MAX, ULONG_WIDTH)
FORMAT_SIGNED_INTEGER_DEFN(LLong, long long, LLONG_MIN, LLONG_MAX, LLONG_WIDTH)
FORMAT_UNSIGNED_INTEGER_DEFN(ULLong, unsigned long long, ULLONG_MAX, ULLONG_WIDTH)

#ifndef TID_Alias_Size
FORMAT_UNSIGNED_INTEGER_DEFN(Size, size_t, SIZE_MAX, SIZE_WIDTH)
#endif

#ifndef TID_Alias_Ptrdiff
FORMAT_SIGNED_INTEGER_DEFN(Ptrdiff, ptrdiff_t, PTRDIFF_MIN, PTRDIFF_MAX, PTRDIFF_WIDTH)
#endif

#ifndef TID_Alias_Intmax
FORMAT_SIGNED_INTEGER_DEFN(Intmax, intmax_t, INTMAX_MIN, INTMAX_MAX, INTMAX_WIDTH)
#endif
#ifndef TID_Alias_UIntmax
FORMAT_UNSIGNED_INTEGER_DEFN(UIntmax, uintmax_t, UINTMAX_MAX, UINTMAX_WIDTH)
#endif

#if defined(INTPTR_MAX) && !defined(TID_Alias_Intptr)
FORMAT_SIGNED_INTEGER_DEFN(Intptr, intptr_t, INTPTR_MIN, INTPTR_MAX, INTPTR_WIDTH)
FORMAT_UNSIGNED_INTEGER_DEFN(UIntptr, uintptr_t, UINTPTR_MAX, UINTPTR_WIDTH)
#endif

#if defined(INT8_MAX) && !defined(TID_Alias_Int8)
FORMAT_SIGNED_INTEGER_DEFN(Int8, int8_t, INT8_MIN, INT8_MAX, INT8_WIDTH)
#endif
#if defined(UINT8_MAX) && !defined(TID_Alias_UInt8)
FORMAT_UNSIGNED_INTEGER_DEFN(UInt8, uint8_t, UINT8_MAX, UINT8_WIDTH)
#endif

#if defined(INT16_MAX) && !defined(TID_Alias_Int16)
FORMAT_SIGNED_INTEGER_DEFN(Int16, int16_t, INT16_MIN, INT16_MAX, INT16_WIDTH)
#endif
#if defined(UINT16_MAX) && !defined(TID_Alias_UInt16)
FORMAT_UNSIGNED_INTEGER_DEFN(UInt16, uint16_t, UINT16_MAX, UINT16_WIDTH)
#endif

#if defined(INT32_MAX) && !defined(TID_Alias_Int32)
FORMAT_SIGNED_INTEGER_DEFN(Int32, int32_t, INT32_MIN, INT32_MAX, INT32_WIDTH)
#endif
#if defined(UINT32_MAX) && !defined(TID_Alias_UInt32)
FORMAT_UNSIGNED_INTEGER_DEFN(UInt32, uint32_t, UINT32_MAX, UINT32_WIDTH)
#endif

#if defined(INT64_MAX) && !defined(TID_Alias_Int64)
FORMAT_SIGNED_INTEGER_DEFN(Int64, int64_t, INT64_MIN, INT64_MAX, INT64_WIDTH)
#endif
#if defined(UINT64_MAX) && !defined(TID_Alias_UInt64)
FORMAT_UNSIGNED_INTEGER_DEFN(UInt64, uint64_t, UINT64_MAX, UINT64_WIDTH)
#endif

#ifndef TID_Alias_Int_Fast8
FORMAT_SIGNED_INTEGER_DEFN(Int_Fast8, int_fast8_t, INT_FAST8_MIN, INT_FAST8_MAX, INT_FAST8_WIDTH)
#endif
#ifndef TID_Alias_UInt_Fast8
FORMAT_UNSIGNED_INTEGER_DEFN(UInt_Fast8, uint_fast8_t, UINT_FAST8_MAX, UINT_FAST8_WIDTH)
#endif

#ifndef TID_Alias_Int_Fast16
FORMAT_SIGNED_INTEGER_DEFN(Int_Fast16, int_fast16_t, INT_FAST16_MIN, INT_FAST16_MAX, INT_FAST16_WIDTH)
#endif
#ifndef TID_Alias_UInt_Fast16
FORMAT_UNSIGNED_INTEGER_DEFN(UInt_Fast16, uint_fast16_t, UINT_FAST16_MAX, UINT_FAST16_WIDTH)
#endif

#ifndef TID_Alias_Int_Fast32
FORMAT_SIGNED_INTEGER_DEFN(Int_Fast32, int_fast32_t, INT_FAST32_MIN, INT_FAST32_MAX, INT_FAST32_WIDTH)
#endif
#ifndef TID_Alias_UInt_Fast32
FORMAT_UNSIGNED_INTEGER_DEFN(UInt_Fast32, uint_fast32_t, UINT_FAST32_MAX, UINT_FAST32_WIDTH)
#endif

#ifndef TID_Alias_Int_Fast64
FORMAT_SIGNED_INTEGER_DEFN(Int_Fast64, int_fast64_t, INT_FAST64_MIN, INT_FAST64_MAX, INT_FAST64_WIDTH)
#endif
#ifndef TID_Alias_UInt_Fast64
FORMAT_UNSIGNED_INTEGER_DEFN(UInt_Fast64, uint_fast64_t, UINT_FAST64_MAX, UINT_FAST64_WIDTH)
#endif

#ifndef TID_Alias_Int_Least8
FORMAT_SIGNED_INTEGER_DEFN(Int_Least8, int_least8_t, INT_LEAST8_MIN, INT_LEAST8_MAX, INT_LEAST8_WIDTH)
#endif
#ifndef TID_Alias_UInt_Least8
FORMAT_UNSIGNED_INTEGER_DEFN(UInt_Least8, uint_least8_t, UINT_LEAST8_MAX, UINT_LEAST8_WIDTH)
#endif

#ifndef TID_Alias_Int_Least16
FORMAT_SIGNED_INTEGER_DEFN(Int_Least16, int_least16_t, INT_LEAST16_MIN, INT_LEAST16_MAX, INT_LEAST16_WIDTH)
#endif
#ifndef TID_Alias_UInt_Least16
FORMAT_UNSIGNED_INTEGER_DEFN(UInt_Least16, uint_least16_t, UINT_LEAST16_MAX, UINT_LEAST16_WIDTH)
#endif

#ifndef TID_Alias_Int_Least32
FORMAT_SIGNED_INTEGER_DEFN(Int_Least32, int_least32_t, INT_LEAST32_MIN, INT_LEAST32_MAX, INT_LEAST32_WIDTH)
#endif
#ifndef TID_Alias_UInt_Least32
FORMAT_UNSIGNED_INTEGER_DEFN(UInt_Least32, uint_least32_t, UINT_LEAST32_MAX, UINT_LEAST32_WIDTH)
#endif

#ifndef TID_Alias_Int_Least64
FORMAT_SIGNED_INTEGER_DEFN(Int_Least64, int_least64_t, INT_LEAST64_MIN, INT_LEAST64_MAX, INT_LEAST64_WIDTH)
#endif
#ifndef TID_Alias_UInt_Least64
FORMAT_UNSIGNED_INTEGER_DEFN(UInt_Least64, uint_least64_t, UINT_LEAST64_MAX, UINT_LEAST64_WIDTH)
#endif

#ifndef TID_Alias_WChar
FORMAT_SIGNED_INTEGER_DEFN(WChar, wchar_t, WCHAR_MIN, WCHAR_MAX, WCHAR_WIDTH)
#endif

int
Format_Integer(struct Writer writer, struct Format_Placeholder fmt, enum Type_ID tid, va_list *args)
{
    int ret = 0;
    unsigned base;
    const char *digits1;
    const char *digits2;
    switch (fmt.specifier) {
    case format_specifier_binary:
        base = 2;
        digits1 = "01";
        digits2 = "00011011";
        break;
    case format_specifier_octal:
        base = 8;
        digits1 = "01234567";
        digits2 = "00010203040506071011121314151617202122232425262730313233343536374041424344454647505152535455565760616263646566677071727374757677";
        break;
    case format_specifier_decimal:
        base = 10;
        digits1 = "0123456789";
        digits2 = "00010203040506070809101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263646566676869707172737475767778798081828384858687888990919293949596979899";
        break;
    case format_specifier_hex:
        base = 16;
        digits1 = "0123456789abcdef";
        digits2 = "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f404142434445464748494a4b4c4d4e4f505152535455565758595a5b5c5d5e5f606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f808182838485868788898a8b8c8d8e8f909192939495969798999a9b9c9d9e9fa0a1a2a3a4a5a6a7a8a9aaabacadaeafb0b1b2b3b4b5b6b7b8b9babbbcbdbebfc0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2d3d4d5d6d7d8d9dadbdcdddedfe0e1e2e3e4e5e6e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfcfdfeff";
        break;
    case format_specifier_Hex:
        base = 16;
        digits1 = "0123456789ABCDEF";
        digits2 = "000102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E1F202122232425262728292A2B2C2D2E2F303132333435363738393A3B3C3D3E3F404142434445464748494A4B4C4D4E4F505152535455565758595A5B5C5D5E5F606162636465666768696A6B6C6D6E6F707172737475767778797A7B7C7D7E7F808182838485868788898A8B8C8D8E8F909192939495969798999A9B9C9D9E9FA0A1A2A3A4A5A6A7A8A9AAABACADAEAFB0B1B2B3B4B5B6B7B8B9BABBBCBDBEBFC0C1C2C3C4C5C6C7C8C9CACBCCCDCECFD0D1D2D3D4D5D6D7D8D9DADBDCDDDEDFE0E1E2E3E4E5E6E7E8E9EAEBECEDEEEFF0F1F2F3F4F5F6F7F8F9FAFBFCFDFEFF";
        break;
    default: unreachable();
    }

    switch (tid) {
        case TIDI(Int):
            ret = Format_Integer_Int(writer, va_arg(*args, int), base, digits1, digits2);
            break;
        case TIDI(UInt):
            ret = Format_Integer_UInt(writer, va_arg(*args, unsigned int), base, digits1, digits2);
            break;
        case TIDI(Long):
            ret = Format_Integer_Long(writer, va_arg(*args, long), base, digits1, digits2);
            break;
        case TIDI(ULong):
            ret = Format_Integer_ULong(writer, va_arg(*args, unsigned long), base, digits1, digits2);
            break;
        case TIDI(LLong):
            ret = Format_Integer_LLong(writer, va_arg(*args, long long), base, digits1, digits2);
            break;
        case TIDI(ULLong):
            ret = Format_Integer_ULLong(writer, va_arg(*args, unsigned long long), base, digits1, digits2);
            break;
        #ifndef TID_Alias_Size
        case TIDI(Size):
            ret = Format_Integer_Size(writer, va_arg(*args, size_t), base, digits1, digits2);
            break;
        #endif
        #ifndef TID_Alias_Ptrdiff
        case TIDI(Ptrdiff):
            ret = Format_Integer_Ptrdiff(writer, va_arg(*args, ptrdiff_t), base, digits1, digits2);
            break;
        #endif
        #ifndef TID_Alias_Intmax
        case TIDI(Intmax):
            ret = Format_Integer_Intmax(writer, va_arg(*args, intmax_t), base, digits1, digits2);
            break;
        #endif
        #ifndef TID_Alias_UIntmax
        case TIDI(UIntmax):
            ret = Format_Integer_UIntmax(writer, va_arg(*args, uintmax_t), base, digits1, digits2);
            break;
        #endif
        #if defined(INTPTR_MAX) && !defined(TID_Alias_Intptr)
        case TIDI(Intptr):
            ret = Format_Integer_Intptr(writer, va_arg(*args, intptr_t), base, digits1, digits2);
            break;
        #endif
        #if defined(UINTPTR_MAX) && !defined(TID_Alias_UIntptr)
        case TIDI(UIntptr):
            ret = Format_Integer_UIntptr(writer, va_arg(*args, uintptr_t), base, digits1, digits2);
            break;
        #endif

        #if defined(INT8_MAX) && !defined(TID_Alias_Int8)
        case TIDI(Int8):
            ret = Format_Integer_Int8(writer, va_arg(*args, int8_t), base, digits1, digits2);
            break;
        #endif
        #if defined(UINT8_MAX) && !defined(TID_Alias_UInt8)
        case TIDI(UInt8):
            ret = Format_Integer_UInt8(writer, va_arg(*args, uint8_t), base, digits1, digits2);
            break;
        #endif

        #if defined(INT16_MAX) && !defined(TID_Alias_Int16)
        case TIDI(Int16):
            ret = Format_Integer_Int16(writer, va_arg(*args, int16_t), base, digits1, digits2);
            break;
        #endif
        #if defined(UINT16_MAX) && !defined(TID_Alias_UInt16)
        case TIDI(UInt16):
            ret = Format_Integer_UInt16(writer, va_arg(*args, uint16_t), base, digits1, digits2);
            break;
        #endif

        #if defined(INT32_MAX) && !defined(TID_Alias_Int32)
        case TIDI(Int32):
            ret = Format_Integer_Int32(writer, va_arg(*args, int32_t), base, digits1, digits2);
            break;
        #endif
        #if defined(UINT32_MAX) && !defined(TID_Alias_UInt32)
        case TIDI(UInt32):
            ret = Format_Integer_UInt32(writer, va_arg(*args, uint32_t), base, digits1, digits2);
            break;
        #endif


        #if defined(INT64_MAX) && !defined(TID_Alias_Int64)
        case TIDI(Int64):
            ret = Format_Integer_Int64(writer, va_arg(*args, int64_t), base, digits1, digits2);
            break;
        #endif
        #if defined(UINT64_MAX) && !defined(TID_Alias_UInt64)
        case TIDI(UInt64):
            ret = Format_Integer_UInt64(writer, va_arg(*args, uint64_t), base, digits1, digits2);
            break;
        #endif

        #ifndef TID_Alias_Int_Fast8
        case TIDI(Int_Fast8):
            ret = Format_Integer_Int_Fast8(writer, va_arg(*args, int_fast8_t), base, digits1, digits2);
            break;
        #endif
        #ifndef TID_Alias_UInt_Fast8
        case TIDI(UInt_Fast8):
            ret = Format_Integer_UInt_Fast8(writer, va_arg(*args, uint_fast8_t), base, digits1, digits2);
            break;
        #endif
        #ifndef TID_Alias_Int_Fast16
        case TIDI(Int_Fast16):
            ret = Format_Integer_Int_Fast16(writer, va_arg(*args, int_fast16_t), base, digits1, digits2);
            break;
        #endif
        #ifndef TID_Alias_UInt_Fast16
        case TIDI(UInt_Fast16):
            ret = Format_Integer_UInt_Fast16(writer, va_arg(*args, uint_fast16_t), base, digits1, digits2);
            break;
        #endif
        #ifndef TID_Alias_Int_Fast32
        case TIDI(Int_Fast32):
            ret = Format_Integer_Int_Fast32(writer, va_arg(*args, int_fast32_t), base, digits1, digits2);
            break;
        #endif
        #ifndef TID_Alias_UInt_Fast32
        case TIDI(UInt_Fast32):
            ret = Format_Integer_UInt_Fast32(writer, va_arg(*args, uint_fast32_t), base, digits1, digits2);
            break;
        #endif
        #ifndef TID_Alias_Int_Fast64
        case TIDI(Int_Fast64):
            ret = Format_Integer_Int_Fast64(writer, va_arg(*args, int_fast64_t), base, digits1, digits2);
            break;
        #endif
        #ifndef TID_Alias_UInt_Fast64
        case TIDI(UInt_Fast64):
            ret = Format_Integer_UInt_Fast64(writer, va_arg(*args, uint_fast64_t), base, digits1, digits2);
            break;
        #endif

        #ifndef TID_Alias_Int_Least8
        case TIDI(Int_Least8):
            ret = Format_Integer_Int_Least8(writer, va_arg(*args, int_least8_t), base, digits1, digits2);
            break;
        #endif
        #ifndef TID_Alias_UInt_Least8
        case TIDI(UInt_Least8):
            ret = Format_Integer_UInt_Least8(writer, va_arg(*args, uint_least8_t), base, digits1, digits2);
            break;
        #endif
        #ifndef TID_Alias_Int_Least16
        case TIDI(Int_Least16):
            ret = Format_Integer_Int_Least16(writer, va_arg(*args, int_least16_t), base, digits1, digits2);
            break;
        #endif
        #ifndef TID_Alias_UInt_Least16
        case TIDI(UInt_Least16):
            ret = Format_Integer_UInt_Least16(writer, va_arg(*args, uint_least16_t), base, digits1, digits2);
            break;
        #endif
        #ifndef TID_Alias_Int_Least32
        case TIDI(Int_Least32):
            ret = Format_Integer_Int_Least32(writer, va_arg(*args, int_least32_t), base, digits1, digits2);
            break;
        #endif
        #ifndef TID_Alias_UInt_Least32
        case TIDI(UInt_Least32):
            ret = Format_Integer_UInt_Least32(writer, va_arg(*args, uint_least32_t), base, digits1, digits2);
            break;
        #endif
        #ifndef TID_Alias_Int_Least64
        case TIDI(Int_Least64):
            ret = Format_Integer_Int_Least64(writer, va_arg(*args, int_least64_t), base, digits1, digits2);
            break;
        #endif
        #ifndef TID_Alias_UInt_Least64
        case TIDI(UInt_Least64):
            ret = Format_Integer_UInt_Least64(writer, va_arg(*args, uint_least64_t), base, digits1, digits2);
            break;
        #endif
        #ifndef TID_Alias_WChar
        case TIDI(WChar):
            ret = Format_Integer_WChar(writer, va_arg(*args, wchar_t), base, digits1, digits2);
            break;
        #endif
        default: unreachable();
    }

    return ret;
}

/// TODO: Support wchar_t?
static inline int
Format_String(struct Writer writer, struct Format_Placeholder fmt, enum Type_ID tid, va_list *args)
{
    int ret;
    if (tid == TID_P_WChar) {
        return E_Writer_Bad_Type;
    }
    const char *str = va_arg(*args, char *);
    switch (fmt.specifier) {
    case format_specifier_string:
        return Format_Char_P(writer, fmt, str);
    default: break;
    }

    unsigned base;
    const char *digits1;
    const char *digits2;
    switch (fmt.specifier) {
    case format_specifier_binary:
        base = 2;
        digits1 = "01";
        digits2 = "00011011";
        break;
    case format_specifier_octal:
        base = 8;
        digits1 = "01234567";
        digits2 = "00010203040506071011121314151617202122232425262730313233343536374041424344454647505152535455565760616263646566677071727374757677";
        break;
    case format_specifier_decimal:
        base = 10;
        digits1 = "0123456789";
        digits2 = "00010203040506070809101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263646566676869707172737475767778798081828384858687888990919293949596979899";
        break;
    case format_specifier_hex:
        base = 16;
        digits1 = "0123456789abcdef";
        digits2 = "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f404142434445464748494a4b4c4d4e4f505152535455565758595a5b5c5d5e5f606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f808182838485868788898a8b8c8d8e8f909192939495969798999a9b9c9d9e9fa0a1a2a3a4a5a6a7a8a9aaabacadaeafb0b1b2b3b4b5b6b7b8b9babbbcbdbebfc0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2d3d4d5d6d7d8d9dadbdcdddedfe0e1e2e3e4e5e6e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfcfdfeff";
        break;
    case format_specifier_Hex:
        base = 16;
        digits1 = "0123456789ABCDEF";
        digits2 = "000102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E1F202122232425262728292A2B2C2D2E2F303132333435363738393A3B3C3D3E3F404142434445464748494A4B4C4D4E4F505152535455565758595A5B5C5D5E5F606162636465666768696A6B6C6D6E6F707172737475767778797A7B7C7D7E7F808182838485868788898A8B8C8D8E8F909192939495969798999A9B9C9D9E9FA0A1A2A3A4A5A6A7A8A9AAABACADAEAFB0B1B2B3B4B5B6B7B8B9BABBBCBDBEBFC0C1C2C3C4C5C6C7C8C9CACBCCCDCECFD0D1D2D3D4D5D6D7D8D9DADBDCDDDEDFE0E1E2E3E4E5E6E7E8E9EAEBECEDEEEFF0F1F2F3F4F5F6F7F8F9FAFBFCFDFEFF";
        break;
    default: unreachable();
    }
    size_t len = 0;
    if (fmt.width < 0) {
        len = strlen(str);
    } else {
        len = strnlen(str, fmt.width);
    }
    ret = Write_All(writer, "( ", 2);
    for (size_t i = 0; i < len; i++) {
        ret = Format_Integer_Int(writer, str[i], base, digits1, digits2);
        if (ret != 0) {
            return ret;
        }
        ret = Write_All(writer, " ", 1);
        if (ret != 0) {
            return ret;
        }
    }
    ret = Write_All(writer, ")", 1);
    return 0;
}


int
Check_Format_TID(enum Type_ID tid, struct Format_Placeholder *placeholder)
{
    switch (tid) {
        case TIDI(Int):
        case TIDI(UInt):
        case TIDI(Long):
        case TIDI(ULong):
        case TIDI(LLong):
        case TIDI(ULLong):
        #ifndef TID_Alias_Size
        case TIDI(Size):
        #endif
        #ifndef TID_Alias_Ptrdiff
        case TIDI(Ptrdiff):
        #endif
        #ifndef TID_Alias_Intmax
        case TIDI(Intmax):
        #endif
        #ifndef TID_Alias_UIntmax
        case TIDI(UIntmax):
        #endif
        #if defined(INTPTR_MAX) && !defined(TID_Alias_Intptr)
        case TIDI(Intptr):
        #endif
        #if defined(UINTPTR_MAX) && !defined(TID_Alias_UIntptr)
        case TIDI(UIntptr):
        #endif
        #if defined(INT8_MAX) && !defined(TID_Alias_Int8)
        case TIDI(Int8):
        #endif
        #if defined(UINT8_MAX) && !defined(TID_Alias_UInt8)
        case TIDI(UInt8):
        #endif
        #if defined(INT16_MAX) && !defined(TID_Alias_Int16)
        case TIDI(Int16):
        #endif
        #if defined(UINT16_MAX) && !defined(TID_Alias_UInt16)
        case TIDI(UInt16):
        #endif
        #if defined(INT32_MAX) && !defined(TID_Alias_Int32)
        case TIDI(Int32):
        #endif
        #if defined(UINT32_MAX) && !defined(TID_Alias_UInt32)
        case TIDI(UInt32):
        #endif
        #if defined(INT64_MAX) && !defined(TID_Alias_Int64)
        case TIDI(Int64):
        #endif
        #if defined(UINT64_MAX) && !defined(TID_Alias_UInt64)
        case TIDI(UInt64):
        #endif
        #ifndef TID_Alias_Int_Fast8
        case TIDI(Int_Fast8):
        #endif
        #ifndef TID_Alias_UInt_Fast8
        case TIDI(UInt_Fast8):
        #endif
        #ifndef TID_Alias_Int_Fast16
        case TIDI(Int_Fast16):
        #endif
        #ifndef TID_Alias_UInt_Fast16
        case TIDI(UInt_Fast16):
        #endif
        #ifndef TID_Alias_Int_Fast32
        case TIDI(Int_Fast32):
        #endif
        #ifndef TID_Alias_UInt_Fast32
        case TIDI(UInt_Fast32):
        #endif
        #ifndef TID_Alias_Int_Fast64
        case TIDI(Int_Fast64):
        #endif
        #ifndef TID_Alias_UInt_Fast64
        case TIDI(UInt_Fast64):
        #endif
        #ifndef TID_Alias_Int_Least8
        case TIDI(Int_Least8):
        #endif
        #ifndef TID_Alias_UInt_Least8
        case TIDI(UInt_Least8):
        #endif
        #ifndef TID_Alias_Int_Least16
        case TIDI(Int_Least16):
        #endif
        #ifndef TID_Alias_UInt_Least16
        case TIDI(UInt_Least16):
        #endif
        #ifndef TID_Alias_Int_Least32
        case TIDI(Int_Least32):
        #endif
        #ifndef TID_Alias_UInt_Least32
        case TIDI(UInt_Least32):
        #endif
        #ifndef TID_Alias_Int_Least64
        case TIDI(Int_Least64):
        #endif
        #ifndef TID_Alias_UInt_Least64
        case TIDI(UInt_Least64):
        #endif
        #ifndef TID_Alias_WChar
        case TIDI(WChar):
        #endif
            {
                switch (placeholder->specifier) {
                    case format_specifier_default:
                        placeholder->specifier = format_specifier_decimal;
                    case format_specifier_binary:
                    case format_specifier_octal:
                    case format_specifier_hex:
                    case format_specifier_Hex:
                    case format_specifier_decimal:
                        placeholder->type = format_type_integer;
                        return 0;
                    default: return E_Writer_Format_Specifier;
                }
            }
            break;
        case TIDP(Char):
        case TIDP(WChar):
            {
                switch (placeholder->specifier) {
                    case format_specifier_default:
                        placeholder->specifier = format_specifier_string;
                    case format_specifier_string:
                    case format_specifier_binary:
                    case format_specifier_octal:
                    case format_specifier_hex:
                    case format_specifier_Hex:
                    case format_specifier_decimal:
                        placeholder->type = format_type_string;
                        return 0;
                    default: return E_Writer_Format_Specifier;
                }
            }
            break;
        case TIDI(Double):
            {
                switch (placeholder->specifier) {
                    case format_specifier_default:
                        placeholder->specifier = format_specifier_decimal;
                    case format_specifier_binary:
                    case format_specifier_octal:
                    case format_specifier_hex:
                    case format_specifier_Hex:
                    case format_specifier_decimal:
                        placeholder->type = format_type_float;
                        return 0;
                    default: return E_Writer_Format_Specifier;
                }
            }
            break;
        default:
            return E_Writer_Bad_Type;
    }
    return 0;
}

/// Do not include { and }
/// *out is indeterminate upon error
static int
Parse_Format_Placeholder(const char *str, size_t len, struct Format_Placeholder *out)
{
    *out = (struct Format_Placeholder){
        .specifier = format_specifier_default,
        .fill = ' ',
        .alignment = format_alignment_left,
        .width = -1,
        .prec = -1,
    };

    if (len == 0) {
        return 0;
    }
    switch (str[0]) {
        case 'b':
            out->specifier = format_specifier_binary;
            break;
        case 'd':
            out->specifier = format_specifier_decimal;
            break;
        case 'o':
            out->specifier = format_specifier_octal;
            break;
        case 'x':
            out->specifier = format_specifier_hex;
            break;
        case 'X':
            out->specifier = format_specifier_Hex;
            break;
        case 'c':
            out->specifier = format_specifier_char;
            break;
        case 's':
            out->specifier = format_specifier_string;
            break;
        case ':':
            break;
        default: return E_Writer_Format_Specifier;
    }

    return 0;
}

/// TODO: Implement
int
Print_Impl(struct Writer writer, const char *format, ...)
{
    int ret = 0;
    const size_t format_len = strlen(format);
    va_list args;
    va_start(args, format);
    va_list tids;
    va_copy(tids, args);

    enum Type_ID current = TIDI(Invalid);
    while ((current = va_arg(args, int)) != TIDI(Sentinel)) {
        if (current == TIDI(Invalid)) {
            ret = E_Writer_Bad_Type;
            goto ret;
        }
    }

    size_t i = 0;
    while (i < format_len) {
        size_t literal_begin = i;
        while (i < format_len) {
            if (format[i] == '{' || format[i] == '}') {
                break;
            }
            i += 1;
        }
        size_t literal_end = i;
        bool unescape_brace = false;

        if (i + 1 < format_len && format[i + 1] == format[i]) {
            unescape_brace = true;
            literal_end += 1;
            i += 2;
        }
        if (literal_begin != literal_end) {
            ret = Write_All(writer, format + literal_begin, literal_end - literal_begin);
            if (ret != 0) {
                goto ret;
            }
        }

        if (unescape_brace) {
            continue;
        }
        if (i >= format_len) {
            break;
        }

        if (format[i] == '}') {
            ret = E_Writer_Format_Start;
            goto ret;
        }
        // skip {
        i += 1;
        size_t fmt_begin = i;
        while (i < format_len && format[i] != '}') {
            i += 1;
        }
        size_t fmt_end = i;
        if (i >= format_len) {
            ret = E_Writer_Format_End;
            goto ret;
        }
        // skip }
        i += 1;

        struct Format_Placeholder placeholder;
        ret = Parse_Format_Placeholder(
            format + fmt_begin,
            fmt_end - fmt_begin,
            &placeholder
        );
        if (ret != 0) {
            goto ret;
        }

        enum Type_ID tid = va_arg(tids, int);

        ret = Check_Format_TID(tid, &placeholder);
        if (ret != 0) {
            goto ret;
        }

        switch (placeholder.type) {
            case format_type_string:
                ret = Format_String(writer, placeholder, tid, &args);
                break;
            case format_type_integer:
                ret = Format_Integer(writer, placeholder, tid, &args);
                break;
            case format_type_float:
                ret = Write_All(writer, "1.0", 3);
                break;
            default: unreachable();
        }

        if (ret != 0) {
            goto ret;
        }

    }
ret:
    va_end(args);
    va_end(tids);
    return ret;
}
