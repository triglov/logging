/*
 * @file: logging.h
 * @author: Paweł Kawula (pawel.kawula@kelectronics.pl)
 * -----
 * Copyright 2025 - KElectronics
 * -----
 */

#ifndef LOGGING_H
#define LOGGING_H

#include <stdarg.h>

#include "logging_levels.h"
#include "logging_stack.h"

/* Version is automatically defined by CMake from project(logging VERSION x.y.z) */
#ifndef LOGGING_VERSION
#define LOGGING_VERSION "unknown"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

typedef int (*Logging_Function_t)(const char *message, ...);

/**
 * @brief Initialize the logging system with a custom logging function.
 * 
 * Sets the function pointer that will be called by all logging macros.
 * The logging function should accept printf-style format strings and arguments.
 * 
 * @param log_func Pointer to the custom logging function that handles output.
 *                 Pass NULL to use a default no-op function (disables logging).
 * 
 * @note Must be called before using any logging macros (LogError, LogWarn, etc.).
 * 
 * @example
 * @code
 * static int my_uart_logger(const char *message, ...) {
 *     va_list args;
 *     va_start(args, message);
 *     uart_vprintf(message, args);
 *     va_end(args);
 *     return 0;
 * }
 * 
 * int main(void) {
 *     Logging_Init(my_uart_logger);
 *     LogInfo("System initialized successfully");
 * }
 * @endcode
 */
void Logging_Init(Logging_Function_t log_func);

/**
 * @brief Initialize the logging system with a custom logging function used in critical situations
 * like HardFault handlers when interrupts are disabled.
 * 
 * Similar to Logging_Init, but intended for use in contexts
 * where interrupts may be disabled. Ensures that the logging function is set
 * safely
*/
void Logging_Init_Critical(Logging_Function_t log_func);

/**
 * @brief Get the version of the logging library.
 * 
 * Returns a string containing the current library version in semantic
 * versioning format (e.g., "1.1.0"). The version is automatically
 * generated from the CMake project version.
 * 
 * @return const char* Version string in format "MAJOR.MINOR.PATCH"
 * 
 * @note The returned string is statically allocated and does not need to be freed.
 * 
 * @example
 * @code
 * printf("Using logging library version: %s\n", Logging_GetVersion());
 * @endcode
 */
const char *Logging_GetVersion(void);

/**
 * @brief Get the name of the logging level.
 * 
 * Returns a string representation of the logging level.
 * 
 * @param level The logging level to get the name for.
 * @return char* String representation of the logging level.
 * 
 * @example
 * @code
 * printf("Current logging level: %s\n", Logging_GetLoggingLevelName(LOG_INFO));
 * @endcode
 */
char *Logging_GetLoggingLevelName(int level);

/**
 * @brief Get the top logging level configured at compile time.
 * 
 * Returns the value of LOGGING_TOP_LOG_LEVEL which defines the highest
 * logging level that will be compiled into the code. Log messages with
 * a level above this will be completely omitted, resulting in zero
 * runtime overhead.
 * 
 * @return int The top logging level (LOG_NONE, LOG_ERROR, LOG_WARN, LOG_INFO, or LOG_DEBUG).
 * 
 * @example
 * @code
 * int top_level = Logging_GetTopLoggingLevel();
 * if (top_level >= LOG_DEBUG) {
 *     LogDebug("Debug logging is enabled");
 * }
 * @endcode
 */
int Logging_GetTopLoggingLevel(void);

#ifdef __cplusplus
}
#endif

#endif /* LOGGING_H */
