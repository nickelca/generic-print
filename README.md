# Example
For an example of this in action, run `./example` to compile and run `example.c`.

# How this works
The general idea is to use _Generic to assign an ID to each type passed to
the Print function. To do this, some C preprocessor crimes are used to apply
this _Generic to each argument passed. The number of arguments this macro can
take is limited by how many macros were hardcoded. To set the limit run
`python generate_foreach.py N` where `N` is the desired limit. The default and
minimum limit is 16.

The print function can then parse the format string and traverse the list of
type IDs to infer the type of each argument.

This project requires C23 (C2X) due to usage of `__VA_OPT__` in macros,
`typeof_unqual` in the _Generic for type ID, enum type specifiers, and _BitInt.
Ensure your compiler is able to support these features. Clang-17 and GCC-13 did
not manage to compile, but Clang-19 did. I haven't tested any other
compilers/versions.
