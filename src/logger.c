#include "cat/logger.h"

#include "cat/internal/functions.h"
#include "cat/internal/severity.h"
#include "cat/memory.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>

// Defines the format used for the logger. Note: It has to include 2 %s statements
#define CAT_LOGGER_FORMAT "[%s]\t%s\n"

// Defines the prefix for the first %s in the CAT_LOGGER_FORMAT
#define CAT_LOGGER_FORMAT_PREFIX_TRACE "T"
#define CAT_LOGGER_FORMAT_PREFIX_DEBUG "D"
#define CAT_LOGGER_FORMAT_PREFIX_INFO "+"
#define CAT_LOGGER_FORMAT_PREFIX_WARN "W"
#define CAT_LOGGER_FORMAT_PREFIX_ERROR "E"
#define CAT_LOGGER_FORMAT_PREFIX_FATAL "-"

typedef enum COLOR_MODE
{
    COLOR_MODE_NONE = 0,
    COLOR_MODE_ANSI = 1,  // Returned from ´isatty´ in unix
} color_mode_e;

static struct
{
    FILE*                        outputLog;
    FILE*                        outputError;
    enum CAT_SEVERITY_VISABILITY visability;
} g_state_s = {};

void catLoggerSetVisabilityLevel(IN enum CAT_SEVERITY_VISABILITY severityLevel)
{
    g_state_s.visability = severityLevel;
}
void catLoggerSetOutputFile(IN FILE* output)
{
    g_state_s.outputLog   = output;
    g_state_s.outputError = output;
}
void catLoggerSetOutputFileLog(IN FILE* output) { g_state_s.outputLog = output; }
void catLoggerSetOutputFileError(IN FILE* output) { g_state_s.outputError = output; }

PRIVATE color_mode_e _GetColorMode(void)
{
    // Only make the calculations once per instance
    static int32_t colorMode = -1;
    if (colorMode != -1)
        return colorMode;

#if defined(__unix__)
    int32_t targetIsTerminal = isatty(fileno(stdout));
    colorMode                = targetIsTerminal;
#else
    colorMode = 0;  // No color
#endif
    return colorMode;
}
PRIVATE const char* _GetColorStringFromColorMode(
    IN enum CAT_SEVERITY_TYPE severity,
    IN color_mode_e           colorMode
)
{
    switch (colorMode)
    {
        case COLOR_MODE_NONE:
            break;

        case COLOR_MODE_ANSI:
            switch (severity)
            {
                case CAT_SEVERITY_TYPE_TRACE:
                    return "\e[0;37m";  // Gray

                case CAT_SEVERITY_TYPE_DEBUG:
                    return "\e[0;35m";  // Purple

                case CAT_SEVERITY_TYPE_INFO:
                    return "\e[0;32m";  // Green

                case CAT_SEVERITY_TYPE_WARN:
                    return "\e[0;33m";  // Yellow

                case CAT_SEVERITY_TYPE_ERROR:
                    return "\e[0;31m";  // Red

                case CAT_SEVERITY_TYPE_FATAL:
                    return "\e[1;91m";  // DarkRed

                case CAT_SEVERITY_TYPE_CUSTOM:
                    break;
            }
            break;
    }
    return "";
}
PRIVATE const char* _GetColorResetFromColorMode(IN color_mode_e colorMode)
{
    static const char* resetString = nullptr;
    if (resetString)
        return resetString;

    switch (colorMode)
    {
        case COLOR_MODE_NONE:
            resetString = "";
            break;

        case COLOR_MODE_ANSI:
            resetString = "\e[0m";
            break;
    }
    return resetString;
}

// Shorthand to perform the va_list and call helper function for Ascii and widechar
#define printA(severity, target, fmt, ...)                      \
    ({                                                          \
        va_list vargs;                                          \
        va_start(vargs, __VA_ARGS__);                           \
        int32_t ret = _PrintA(severity, target, fmt, vargs);    \
        va_end(vargs);                                          \
        ret;                                                    \
    })

#define printW(severity, target, fmt, ...)                      \
    ({                                                          \
        va_list vargs;                                          \
        va_start(vargs, __VA_ARGS__);                           \
        int32_t ret = _PrintW(severity, target, fmt, vargs);    \
        va_end(vargs);                                          \
        ret;                                                    \
    })

PRIVATE int32_t _WriteA(FILE* output, const char* msg)
{
    // Output the given message to output file
    return fprintf(output, "%s", msg);
}
PRIVATE int32_t _WriteW(FILE* output, const wchar_t* msg, ...)
{
    // Output the given message to output file as widechar
    return fprintf(output, "%ls", msg);
}


PRIVATE int32_t _PrintA_Impl(
    FILE*       output,
    const char* format,
    const char* prefix,
    const char* msg,
    va_list     vargs
)
{
    // Check if we can use the buffer, otherwise dynamically allocate
    va_list vacpy;
    va_copy(vacpy, vargs);
    size_t msgSize = vsnprintf(nullptr, 0, msg, vacpy) + 1;
    va_end(vacpy);

    static char*    buffer         = nullptr;
    static uint32_t bufferCapacity = 0;

    size_t totalMsgSize = msgSize + strlen(format) + strlen(prefix);
    if (totalMsgSize > bufferCapacity)
    {
        bufferCapacity = totalMsgSize;
        void* tmp      = realloc(buffer, bufferCapacity);
        if (tmp == nullptr)
        {
            int32_t ret = fprintf(
                output,
                "[" CAT_LOGGER_FORMAT_PREFIX_FATAL
                "]\tUnable to allocate print memory for CAT logger. Aborting\n msg:%s",
                msg
            );
            abort();  // TODO: Could resolve this in another way that does not abort the process
            return ret;
        }
        buffer = tmp;
    }
    // We now assume that the buffer has enough capacity for the expanded message and format

    // 1. Put msg into the format
    raii char* scratchBuffer = malloc(bufferCapacity);
    if (scratchBuffer == nullptr)
    {
        int32_t ret = fprintf(
            output,
            "[" CAT_LOGGER_FORMAT_PREFIX_FATAL
            "]\tUnable to allocate print memory for CAT logger. Aborting\n msg:%s",
            msg
        );
        abort();  // TODO: Could resolve this in another way that does not abort the process
        return ret;
    }
    size_t size = snprintf(scratchBuffer, bufferCapacity, format, prefix, msg);
    if (size < 1)
    {
        int32_t ret = fprintf(
            output,
            "[" CAT_LOGGER_FORMAT_PREFIX_ERROR "Error generating message: %s",
            msg
        );
        return ret;
    }

    // 2. Resolve the msg vargs
    size = vsnprintf(buffer, bufferCapacity, scratchBuffer, vargs);
    if (size < 1)
    {
        int32_t ret = fprintf(
            output,
            "[" CAT_LOGGER_FORMAT_PREFIX_ERROR "Error generating message: %s",
            msg
        );
        return ret;
    }
    // Ensure the NULL terminator is in place instead of garbage memory
    buffer[size] = '\0';

    // Finally write to output
    return _WriteA(output, buffer);
}

// clang-format off
PRIVATE int32_t _PrintA(
    enum CAT_SEVERITY_TYPE severity,
    FILE*                  output,
    const char*            msg,
    va_list                vargs
)  // clang-format on
{
    const char* fmt    = CAT_LOGGER_FORMAT;
    const char* prefix = "";
    switch (severity)
    {
        case CAT_SEVERITY_TYPE_TRACE:
            prefix = CAT_LOGGER_FORMAT_PREFIX_TRACE;
            break;

        case CAT_SEVERITY_TYPE_DEBUG:
            prefix = CAT_LOGGER_FORMAT_PREFIX_DEBUG;
            break;

        case CAT_SEVERITY_TYPE_INFO:
            prefix = CAT_LOGGER_FORMAT_PREFIX_INFO;
            break;

        case CAT_SEVERITY_TYPE_WARN:
            prefix = CAT_LOGGER_FORMAT_PREFIX_WARN;
            break;

        case CAT_SEVERITY_TYPE_ERROR:
            prefix = CAT_LOGGER_FORMAT_PREFIX_ERROR;
            break;

        case CAT_SEVERITY_TYPE_FATAL:
            _PrintA_Impl(output, fmt, CAT_LOGGER_FORMAT_PREFIX_FATAL, msg, vargs);
            abort();
            break;

        case CAT_SEVERITY_TYPE_CUSTOM:
            fmt = "%s%s";
            break;
    }
    return _PrintA_Impl(output, fmt, prefix, msg, vargs);
}

// clang-format off
PRIVATE int32_t _PrintW(
    enum CAT_SEVERITY_TYPE severity,
    FILE*                  output,
    const wchar_t*         msg,
    va_list                vargs
)  // clang-format on
{
    //
}


PRIVATE inline FILE* _GetOutputTargetFromSeverity(enum CAT_SEVERITY_TYPE severity)
{
    if (severity == CAT_SEVERITY_TYPE_CUSTOM || severity < CAT_SEVERITY_TYPE_WARN)
        return g_state_s.outputLog != nullptr ? g_state_s.outputLog : stdout;
    return g_state_s.outputError != nullptr ? g_state_s.outputError : stderr;
}


// =============================== INTERFACE =================================

int32_t catPrintA(IN enum CAT_SEVERITY_TYPE severity, IN const char* fmt, OPT...)
{
    int32_t      ret       = 0;
    color_mode_e colorMode = _GetColorMode();
    FILE*        output    = _GetOutputTargetFromSeverity(severity);

    // Set colormode
    _WriteA(output, _GetColorStringFromColorMode(severity, colorMode));

    ret = printA(severity, output, fmt, ...);

    // Reset colormode
    _WriteA(output, _GetColorResetFromColorMode(colorMode));
    return ret;
}
int32_t catPrintANc(IN enum CAT_SEVERITY_TYPE severity, IN const char* fmt, OPT...)
{
    FILE* output = _GetOutputTargetFromSeverity(severity);
    return printA(severity, output, fmt, ...);
}
int32_t catPrintAEx(IN enum CAT_SEVERITY_TYPE severity, IN FILE* output, IN const char* fmt, OPT...)
{
    return printA(severity, output, fmt, ...);
}


int32_t catPrintW(IN enum CAT_SEVERITY_TYPE severity, IN const wchar_t* fmt, OPT...)
{
    //
}
int32_t catPrintWNc(IN enum CAT_SEVERITY_TYPE severity, IN const wchar_t* fmt, OPT...)
{
    //
}
int32_t catPrintWEx(IN enum CAT_SEVERITY_TYPE severity, IN FILE* output, IN const char* fmt, OPT...)
{
    //
}


int32_t catFileTraceA(IN const char* fmt, OPT...)
{
    //
}
int32_t catFileTraceAEx(IN FILE* output, IN const char* fmt, OPT...)
{
    //
}
int32_t catFileTraceW(IN const wchar_t* fmt, OPT...)
{
    //
}
int32_t catFileTraceWEx(IN FILE* output, IN const wchar_t* fmt, OPT...)
{
    //
}
int32_t catFileTraceFlush(void)
{
    //
}
int32_t catFileTraceNewFrame(void)
{
    //
}
