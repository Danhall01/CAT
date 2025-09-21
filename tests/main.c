// This test ensures that the library can be included from the native language
#include "cat.h"
#include "cat/logger.h"

int main(void)
{
    catTrace("Hello C!");
    catLoggerSetVisabilityLevel(4);
    catTrace("Not shown");
    catError("Shown");

    return 0;
}
