# CAT
C 
Auxiliary
Tools

...

## Logging
Each function will have corresponding versions of `catLogA` for ASCII, `catLogW` for widechars, as well as a `Nc` extention for no automatic coloring.
The functions also include an `Ex` version which allows you to choose the file which the log function should print to. This is defaulted to either `stdout` or `stderr` depending on severity.


### Macros
The macros changes the default behaviour of the logger functions and should be included before `#include <cat/logger.h>`.
* `CAT_LOGGER_USE_WIDECHAR`: Changes the default behaviour of the logger to use widechar instead of ASCII.
* `CAT_LOGGER_DISABLE_COLOR`: Disables the default behaviour to use colors with severity type.
* `CAT_LOGGER_DISABLE_FRAME_TRACE`: Disables all of the frame trace functions.

### Options
You can change the default behaviour of where the log functions print by invoking the `catLoggerSetOutputFileLog` and `catLoggerSetOutputFileError` functions respectively.

The default option is the display all log messages regardless of severity level. This behaviour can also be changed by invoking the `catLoggerSetVisabilityLevel` function. This function takes the level of severity to be displayed. For example, to make the logger only output severity or error or above, you can add the following to the application.
```c
#include <cat/loggger.h>
int main(void)
{
    catTrace("Will be printed!");
    catLoggerSetVisabilityLevel(CAT_ERROR_VISABILITY_ERROR);
    catTrace("This is no longer printed!");
    catError("This will still be printed!");
}
```
