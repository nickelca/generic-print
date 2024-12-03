#ifndef WRITER_H
#define WRITER_H

#include "for_each.h"
#include "type_id.h"
#include "type_id_common.h"
#include <stddef.h>

// clang-format off

enum E_Writer {
    /// Full count of bytes was not successfully written.
    E_Writer_Short_Count = 1,
    /// An invalid type was passed to Print
    E_Writer_Bad_Type,
    /// An object is missing from Print
    E_Writer_Object_Missing,
    /// An object is extra from Print
    E_Writer_Object_Extra,
    /// Format placeholder is missing opening {.
    E_Writer_Format_Start,
    /// Format placeholder is missing closing }.
    E_Writer_Format_End,
    /// Format placeholder has an invalid specifier.
    E_Writer_Format_Specifier,
    /// Format placeholder has invalid alignment character (not '<', '^', '>').
    E_Writer_Format_Alignment,
    /// Format placeholder has invalid width, e.g. not a number.
    E_Writer_Format_Width,
    /// Format placeholder has invalid precision, e.g. not a number.
    E_Writer_Format_Precision,
    /// Malloc call failed.
    E_Writer_Malloc,
};

struct Writer {
    void *ctx;
    size_t (*write_fn)(void *ctx, const char *buf, size_t count);
};

/// Get writer pointing to stdout
struct Writer
Get_STD_Out(void);

/// Write up to count bytes from buf
///
/// TODO: Find proper error values
size_t
Write(struct Writer writer, const char *buf, size_t count);

/// Write count bytes from buf, erroring if full count could not be written
///
/// On success returns 0.
/// On failure returns non-zero value.
int
Write_All(struct Writer writer, const char *buf, size_t count);

/// Print a formatted string.
/// Placeholders follow the format:
/// {[specifier]:[fill][alignment][width].[precision]}.
/// [length modifier] modifies the length of the type specified by [specifier]
/// [specifier] determines how an argument should be formatted.
/// [fill] is a single char which is used to pad the text.
/// [alignment] is one of '<', '^', '>' to make the text left-, center-, or
/// right-aligned, respectively.
/// [width] is the total width of the field.
/// [precision] specifies how many decimals a formatted floating point number
/// should have.
/// TODO: Implement [fill] [alignment] [width] [precision]
///
/// [specifier] can be one of:
/// b    Print number as binary (base 2).
/// o    Print number as octal (base 8).
/// d    Print number as decimal (base 10).
/// x    Print number as hexadecimal (base 16) with lowercase letters.
/// X    Print number as hexadecimal (base 16) with uppercase letters.
/// c    Print number as a character. TODO: Implement this
/// s    Print null-terminated string
///
/// To print literal { and }, escape them by writing them twice, e.g. {{ or }}.
///
/// On success returns 0.
/// On failure returns non-zero value.
///
int
Print_Impl(struct Writer writer, const char *format, ...);

#define TYPE_ID_FOR_EACH(x) TYPE_ID(x),
#define Print(writer, fmt, ...)                                                \
    Print_Impl(                                                                \
        writer,                                                                \
        fmt,                                                                   \
        FOR_EACH(TYPE_ID_FOR_EACH, __VA_ARGS__) TIDI(Sentinel) __VA_OPT__(, )  \
        __VA_ARGS__                                                            \
    )

#endif
