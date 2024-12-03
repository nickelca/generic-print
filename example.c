#include "include/writer.h"

int main(void) {
    int ret;
    auto const stdout = Get_STD_Out();
    ret = Print(
        stdout,
        "Hello, {X}. You have {x} dollars and {b} {}\n",
        "Ink",
        23llu,
        88,
        "Oranges"
    );

    if (ret != 0) {
        return ret;
    }

    return 0;
}
