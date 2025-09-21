#ifndef _INCLUDE_CAT_LOGGER_H_
#define _INCLUDE_CAT_LOGGER_H_

#include "internal/functions.h"
#include "internal/severity.h"

#include <stdint.h>
#include <wchar.h>


// Sets which visability level will be used for printing.
// Frame trace uses can be disabled with defining `CAT_LOGGER_DISABLE_FRAME_TRACE`.
CAT_API void catLoggerSetVisabilityLevel(IN enum CAT_SEVERITY_VISABILITY severityLevel);

// Sets the default output file for all messages.
CAT_API void catLoggerSetOutputFile(IN FILE* output);

// Sets the default output file for non error messages.
CAT_API void catLoggerSetOutputFileLog(IN FILE* output);

// Sets the default output file for error messages.
CAT_API void catLoggerSetOutputFileError(IN FILE* output);


// Print a Severity level message to the selected output file (default stdout).
CAT_API int32_t catPrintA(IN enum CAT_SEVERITY_TYPE severity, IN const char* fmt, OPT...);
// Print a Severity level message without color to the selected output file (default stdout).
CAT_API int32_t catPrintANc(IN enum CAT_SEVERITY_TYPE severity, IN const char* fmt, OPT...);
// Print a Severity level message to the selected output file with no colors enabled.
CAT_API int32_t
    catPrintAEx(IN enum CAT_SEVERITY_TYPE severity, IN FILE* output, IN const char* fmt, OPT...);

// Print a Severity level widechar message to the selected output file (default stdout).
CAT_API int32_t catPrintW(IN enum CAT_SEVERITY_TYPE severity, IN const wchar_t* fmt, OPT...);
// Print a Severity level widechar message without colors to the selected output file (default
// stdout).
CAT_API int32_t catPrintWNc(IN enum CAT_SEVERITY_TYPE severity, IN const wchar_t* fmt, OPT...);
// Print a Severity level widechar message to the selected output File with no colors enabled.
CAT_API int32_t
    catPrintWEx(IN enum CAT_SEVERITY_TYPE severity, IN FILE* output, IN const char* fmt, OPT...);


// Debug level messages will only be defined if the flag _DEBUG is defined, otherwise do nothing.
#ifndef _DEBUG
    #define catDebug(fmt, ...)
#endif

#ifdef CAT_LOGGER_USE_WIDECHAR
    #ifdef CAT_LOGGER_DISABLE_COLOR
        #define catPrint(fmt, ...) catPrintWNc(CAT_SEVERITY_TYPE_CUSTOM, fmt __VA_OPT__(,) __va_ARGS__)
        #define catTrace(fmt, ...) catPrintWNc(CAT_SEVERITY_TYPE_TRACE, fmt __VA_OPT__(,) __VA_ARGS__)
        #define catInfo(fmt, ...) catPrintWNc(CAT_SEVERITY_TYPE_INFO, fmt __VA_OPT__(,) __VA_ARGS__)
        #define catWarn(fmt, ...) catPrintWNc(CAT_SEVERITY_TYPE_WARN, fmt __VA_OPT__(,) __VA_ARGS__)
        #define catError(fmt, ...) catPrintWNc(CAT_SEVERITY_TYPE_ERROR, fmt __VA_OPT__(,) __VA_ARGS__)
        #define catFatal(fmt, ...) catPrintWNc(CAT_SEVERITY_TYPE_FATAL, fmt __VA_OPT__(,) __VA_ARGS__)

        #ifdef _DEBUG
            #define catDebug(fmt, ...) catPrintWNc(CAT_SEVERITY_TYPE_DEBUG, fmt __VA_OPT__(,) __VA_ARGS__)
        #endif
    #else
        #define catPrint(fmt, ...) catPrintW(CAT_SEVERITY_TYPE_CUSTOM, fmt __VA_OPT__(,) __VA_ARGS__)
        #define catTrace(fmt, ...) catPrintW(CAT_SEVERITY_TYPE_TRACE, fmt __VA_OPT__(,) __VA_ARGS__)
        #define catInfo(fmt, ...) catPrintW(CAT_SEVERITY_TYPE_INFO, fmt __VA_OPT__(,) __VA_ARGS__)
        #define catWarn(fmt, ...) catPrintW(CAT_SEVERITY_TYPE_WARN, fmt __VA_OPT__(,) __VA_ARGS__)
        #define catError(fmt, ...) catPrintW(CAT_SEVERITY_TYPE_ERROR, fmt __VA_OPT__(,) __VA_ARGS__)
        #define catFatal(fmt, ...) catPrintW(CAT_SEVERITY_TYPE_FATAL, fmt __VA_OPT__(,) __VA_ARGS__)

        #ifdef _DEBUG
            #define catDebug(fmt, ...) catPrintW(CAT_SEVERITY_TYPE_DEBUG, fmt __VA_OPT__(,) __VA_ARGS__)
        #endif
    #endif

    // Default no color functions
    #define catDebugNc(fmt, ...) catPrintWNc(CAT_SEVERITY_TYPE_CUSTOM, fmt __VA_OPT__(,) __VA_ARGS__)
    #define catTraceNc(fmt, ...) catPrintWNc(CAT_SEVERITY_TYPE_TRACE, fmt __VA_OPT__(,) __VA_ARGS__)
    #define catInfoNc(fmt, ...) catPrintWNc(CAT_SEVERITY_TYPE_INFO, fmt __VA_OPT__(,) __VA_ARGS__)
    #define catWarnNc(fmt, ...) catPrintWNc(CAT_SEVERITY_TYPE_WARN, fmt __VA_OPT__(,) __VA_ARGS__)
    #define catErrorNc(fmt, ...) catPrintWNc(CAT_SEVERITY_TYPE_ERROR, fmt __VA_OPT__(,) __VA_ARGS__)
    #define catFatalNc(fmt, ...) catPrintWNc(CAT_SEVERITY_TYPE_FATAL, fmt __VA_OPT__(,) __VA_ARGS__)

    // Due to output not being default we do not apply colors
    #define catPrintEx(file, fmt, ...) catPrintWEx(CAT_SEVERITY_TYPE_CUSTOM, file, fmt __VA_OPT__(,) __VA_ARGS__)
    #define catTraceEx(file, fmt, ...) catPrintWEx(CAT_SEVERITY_TYPE_TRACE, file, fmt __VA_OPT__(,) __VA_ARGS__)
    #define catInfoEx(file, fmt, ...) catPrintWEx(CAT_SEVERITY_TYPE_INFO,  file, fmt __VA_OPT__(,) __VA_ARGS__)
    #define catWarnEx(file, fmt, ...) catPrintWEx(CAT_SEVERITY_TYPE_WARN, file, fmt __VA_OPT__(,) __VA_ARGS__)
    #define catErrorEx(file, fmt, ...) catPrintWEx(CAT_SEVERITY_TYPE_ERROR, file, fmt __VA_OPT__(,) __VA_ARGS__)
    #define catFatalEx(file, fmt, ...) catPrintWEx(CAT_SEVERITY_TYPE_FATAL, file, fmt __VA_OPT__(,) __VA_ARGS__)

    #ifdef _DEBUG
        #define catDebugNc(fmt, ...) catPrintWNc(CAT_SEVERITY_TYPE_DEBUG, fmt __VA_OPT__(,) __VA_ARGS__)
        #define catDebugEx(file, fmt, ...) catPrintWEx(CAT_SEVERITY_TYPE_DEBUG, file, fmt __VA_OPT__(,) __VA_ARGS__)
    #endif
#else
    #ifdef CAT_LOGGER_DISABLE_COLOR
        #define catPrint(fmt, ...) catPrintANc(CAT_SEVERITY_TYPE_CUSTOM, fmt __VA_OPT__(,) __VA_ARGS__)
        #define catTrace(fmt, ...) catPrintANc(CAT_SEVERITY_TYPE_TRACE, fmt __VA_OPT__(,) __VA_ARGS__)
        #define catInfo(fmt, ...) catPrintANc(CAT_SEVERITY_TYPE_INFO, fmt __VA_OPT__(,) __VA_ARGS__)
        #define catWarn(fmt, ...) catPrintANc(CAT_SEVERITY_TYPE_WARN, fmt __VA_OPT__(,) __VA_ARGS__)
        #define catError(fmt, ...) catPrintANc(CAT_SEVERITY_TYPE_ERROR, fmt __VA_OPT__(,) __VA_ARGS__)
        #define catFatal(fmt, ...) catPrintANc(CAT_SEVERITY_TYPE_FATAL, fmt __VA_OPT__(,) __VA_ARGS__)
        #ifdef _DEBUG
            #define catDebug(fmt, ...) catPrintANc(CAT_SEVERITY_TYPE_DEBUG, fmt __VA_OPT__(,) __VA_ARGS__)
        #endif
    #else
        #define catPrint(fmt, ...) catPrintA(CAT_SEVERITY_TYPE_CUSTOM, fmt __VA_OPT__(,) __VA_ARGS__)
        #define catTrace(fmt, ...) catPrintA(CAT_SEVERITY_TYPE_TRACE, fmt __VA_OPT__(,) __VA_ARGS__)
        #define catInfo(fmt, ...) catPrintA(CAT_SEVERITY_TYPE_INFO, fmt __VA_OPT__(,) __VA_ARGS__)
        #define catWarn(fmt, ...) catPrintA(CAT_SEVERITY_TYPE_WARN, fmt __VA_OPT__(,) __VA_ARGS__)
        #define catError(fmt, ...) catPrintA(CAT_SEVERITY_TYPE_ERROR, fmt __VA_OPT__(,) __VA_ARGS__)
        #define catFatal(fmt, ...) catPrintA(CAT_SEVERITY_TYPE_FATAL, fmt __VA_OPT__(,) __VA_ARGS__)
        #ifdef _DEBUG
            #define catDebug(fmt, ...) catPrintA(CAT_SEVERITY_TYPE_DEBUG, fmt __VA_OPT__(,) __VA_ARGS__)
        #endif
    #endif

    // Default no color functions.
    #define catPrintNc(fmt, ...) catPrintANc(CAT_SEVERITY_TYPE_CUSTOM, fmt __VA_OPT__(,) __VA_ARGS__)
    #define catTraceNc(fmt, ...) catPrintANc(CAT_SEVERITY_TYPE_TRACE, fmt __VA_OPT__(,) __VA_ARGS__)
    #define catInfoNc(fmt, ...) catPrintANc(CAT_SEVERITY_TYPE_INFO, fmt __VA_OPT__(,) __VA_ARGS__)
    #define catWarnNc(fmt, ...) catPrintANc(CAT_SEVERITY_TYPE_WARN, fmt __VA_OPT__(,) __VA_ARGS__)
    #define catErrorNc(fmt, ...) catPrintANc(CAT_SEVERITY_TYPE_ERROR, fmt __VA_OPT__(,) __VA_ARGS__)
    #define catFatalNc(fmt, ...) catPrintANc(CAT_SEVERITY_TYPE_FATAL, fmt __VA_OPT__(,) __VA_ARGS__)

    // Due to output not being default we do not apply colors.
    #define catPrintEx(file, fmt, ...) catPrintAEx(CAT_SEVERITY_TYPE_CUSTOM, file, fmt __VA_OPT__(,) __VA_ARGS__)
    #define catTraceEx(file, fmt, ...) catPrintAEx(CAT_SEVERITY_TYPE_TRACE, file, fmt __VA_OPT__(,) __VA_ARGS__)
    #define catInfoEx(file, fmt, ...) catPrintAEx(CAT_SEVERITY_TYPE_INFO,  file, fmt __VA_OPT__(,) __VA_ARGS__)
    #define catWarnEx(file, fmt, ...) catPrintAEx(CAT_SEVERITY_TYPE_WARN, file, fmt __VA_OPT__(,) __VA_ARGS__)
    #define catErrorEx(file, fmt, ...) catPrintAEx(CAT_SEVERITY_TYPE_ERROR, file, fmt __VA_OPT__(,) __VA_ARGS__)
    #define catFatalEx(file, fmt, ...) catPrintAEx(CAT_SEVERITY_TYPE_FATAL, file, fmt __VA_OPT__(,) __VA_ARGS__)

    #ifdef _DEBUG
        #define catDebugNc(fmt, ...) catPrintANc(CAT_SEVERITY_TYPE_DEBUG, fmt __VA_OPT__(,) __VA_ARGS__)
        #define catDebugEx(file, fmt, ...) catPrintAEx(CAT_SEVERITY_TYPE_DEBUG, file, fmt __VA_OPT__(,) __VA_ARGS__)
    #endif
#endif


#ifndef CAT_LOGGER_DISABLE_FRAME_TRACE
// Print a frame trace level message to the default output file (default stdout). This message is
// also written to .dump file upon exiting the application or on catTraceFlush function call.
CAT_API int32_t catFileTraceA(IN const char* fmt, OPT...);

// Print a frame trace level message to the selected output file. This message is also written to
// .dump file upon exiting the application or on catTraceFlush function call.
CAT_API int32_t catFileTraceAEx(IN FILE* output, IN const char* fmt, OPT...);

// Print a frame trace level widechar message to the default output file (default stdout). This
// message is also written to .dump file upon exiting the application or on catTraceFlush function
// call.
CAT_API int32_t catFileTraceW(IN const wchar_t* fmt, OPT...);

// Print a frame trace level widechar message to the selected output file. This message is also
// written to .dump file upon exiting the application or on catTraceFlush function call.
CAT_API int32_t catFileTraceWEx(IN FILE* output, IN const wchar_t* fmt, OPT...);

// Flush the frame trace to console.
CAT_API int32_t catFileTraceFlush(void);

// Reset the frame trace to begining of frame.
CAT_API int32_t catFileTraceNewFrame(void);

    #ifdef CAT_LOGGER_USE_WIDECHAR
        #define catFileTrace(fmt, ...) catFileTraceW(fmt __VA_OPT__(,) __VA_ARGS__)
        #define catFileTraceEx(file, fmt, ...) catFileTraceWEx(file, fmt __VA_OPT__(,) __VA_ARGS__)
    #else
        #define catFileTrace(fmt, ...) catFileTraceA(fmt __VA_OPT__(,) __VA_ARGS__)
        #define catFileTraceEx(file, fmt, ...) catFileTraceAEx(file, fmt __VA_OPT__(,) __VA_ARGS__)
    #endif

#else
    #define catFileTrace(fmt, ...)
    #define catFileTraceEx(file, fmt, ...)

    #define catFileTraceA(fmt, ...)
    #define catFileTraceAEx(fmt, ...)

    #define catFileTraceW(fmt, ...)
    #define catFileTraceWEx(fmt, ...)

    #define catFileTraceFlush()
    #define catFileTraceNewFrame()
#endif

#endif /* _INCLUDE_CAT_LOGGER_H_ */
