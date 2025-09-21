// This test ensures that the library is portable to C++
#include "cat.h"

auto main() -> int
{
    catTrace("Hello C++!");
    return 0;
}
