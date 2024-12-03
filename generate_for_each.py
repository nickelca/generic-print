import sys

arg_count = 16
try:
    arg_count = int(sys.argv[1])
    if arg_count < 16:
        arg_count = 16
except ValueError:
    arg_count = 16

FOR_EACH = "\n".join([
    f"#define FOR_EACH_{n}(macro, x, ...) macro(x) FOR_EACH_{n-1}(macro, __VA_ARGS__)"
    for n in range(1, arg_count + 1)
])

FOR_EACH_ARG_N = "#define FOR_EACH_ARG_N(" + ", ".join([
    f"_{n}"
    for n in range(1, arg_count + 1)
]) + ", N, ...) N"

FOR_EACH_RSEQ_N = "#define FOR_EACH_RSEQ_N() " + ", ".join(map(str, reversed(range(arg_count + 1))))

with open("./include/for_each.h", "wt") as f:
    f.write(f"""#ifndef FOR_EACH_H
#define FOR_EACH_H

#define FOR_EACH_CONCAT(a, b) FOR_EACH_CONCAT_1(a, b)
#define FOR_EACH_CONCAT_1(a, b) a##b

#define FOR_EACH_0(macro, x, ...)
{FOR_EACH}

#define FOR_EACH_NARG(...)                                                     \\
    FOR_EACH_NARG_(__VA_ARGS__ __VA_OPT__(, ) FOR_EACH_RSEQ_N())
#define FOR_EACH_NARG_(...) FOR_EACH_ARG_N(__VA_ARGS__)
{FOR_EACH_ARG_N}
{FOR_EACH_RSEQ_N}

#define FOR_EACH_(N, macro, x, ...)                                            \\
    FOR_EACH_CONCAT(FOR_EACH_, N)(macro, x, __VA_ARGS__)
#define FOR_EACH(macro, x, ...)                                                \\
    FOR_EACH_(                                                                 \\
        FOR_EACH_NARG(x __VA_OPT__(, ) __VA_ARGS__), macro, x, __VA_ARGS__     \\
    )

#endif
""")
