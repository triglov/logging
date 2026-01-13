/**
 * @file: logging_stack.h  
 * @author: Paweł Kawula (pawel.kawula@kelectronics.pl)
 * @brief: High-performance embedded logging system with compile-time optimization
 * -----
 * Copyright 2025 - KElectronics
 * -----
 * @note: Core logging concepts inspired by FreeRTOS logging framework,
 *        but implementation completely rewritten for embedded performance
 */

#ifndef LOGGING_STACK_H
#define LOGGING_STACK_H

#include "logging_levels.h"

#define LOGGING_STRINGIZE(x) LOGGING_STRINGIZE2(x)
#define LOGGING_STRINGIZE2(x) #x
#define LOGGING_LINE_STRING LOGGING_STRINGIZE(__LINE__)

#ifndef FILE_NAME
#define FILE_NAME ""
#endif

/* Compile-time string concatenation */
#ifdef LOGGING_LOG_NAME
#ifdef LOGGING_PRINT_FILE_PATH
#define LOG_PREFIX "[" LOGGING_LOG_NAME "] (" __FILE__ ") "
#else
#define LOG_PREFIX "[" LOGGING_LOG_NAME "] "
#endif
#else
#ifdef LOGGING_PRINT_FILE_PATH
#define LOG_PREFIX "(" __FILE__ ") "
#else
#define LOG_PREFIX ""
#endif
#endif

#define LOG_SUFFIX ":" LOGGING_LINE_STRING " - "

/* Function name optimization */
#ifdef LOGGING_PRINT_FUNCTION_NAME
#define LOG_WITH_FUNC(level, message, ...) \
    SdkLog(level LOG_PREFIX "(%s)" LOG_SUFFIX message "\r\n", __func__, ##__VA_ARGS__)
#define LOG_WITHOUT_FUNC(level, message, ...) \
    LOG_WITH_FUNC(level, message, ##__VA_ARGS__)
#else
#define LOG_WITH_FUNC(level, message, ...) \
    SdkLog(level LOG_PREFIX LOG_SUFFIX message "\r\n", ##__VA_ARGS__)
#define LOG_WITHOUT_FUNC(level, message, ...) \
    LOG_WITH_FUNC(level, message, ##__VA_ARGS__)
#endif

#define CRITICAL_LOG(level, message, ...) \
    SdkLog_Critical(level LOG_PREFIX LOG_SUFFIX message "\r\n", ##__VA_ARGS__)

extern int (*log_function)(const char *message, ...);
extern int (*critical_log_function)(const char *message, ...);

#if !defined(LOGGING_DISABLED_GLOBALLY)
#define SdkLog(message, ...) log_function(message, ##__VA_ARGS__)
#define SdkLog_Critical(message, ...) critical_log_function(message, ##__VA_ARGS__)
#else
#define SdkLog(message, ...)
#define SdkLog_Critical(message, ...)
#endif

/* Log level validation */
#if !defined(LOGGING_TOP_LOG_LEVEL) ||          \
    ((LOGGING_TOP_LOG_LEVEL != LOG_NONE) &&     \
     (LOGGING_TOP_LOG_LEVEL != LOG_CRITICAL) && \
     (LOGGING_TOP_LOG_LEVEL != LOG_ERROR) &&    \
     (LOGGING_TOP_LOG_LEVEL != LOG_WARN) &&     \
     (LOGGING_TOP_LOG_LEVEL != LOG_INFO) &&     \
     (LOGGING_TOP_LOG_LEVEL != LOG_DEBUG))
#error "Please define LOGGING_TOP_LOG_LEVEL as either LOG_NONE, LOG_CRITICAL, LOG_ERROR, LOG_WARN, LOG_INFO, or LOG_DEBUG."
#else
#if LOGGING_TOP_LOG_LEVEL == LOG_DEBUG
#define LogCritical(message, ...) CRITICAL_LOG("[CRITICAL] ", message, ##__VA_ARGS__)
#define LogError(message, ...) LOG_WITH_FUNC("[ERROR] ", message, ##__VA_ARGS__)
#define LogWarn(message, ...) LOG_WITH_FUNC("[WARN]  ", message, ##__VA_ARGS__)
#define LogInfo(message, ...) LOG_WITH_FUNC("[INFO]  ", message, ##__VA_ARGS__)
#define LogDebug(message, ...) LOG_WITH_FUNC("[DEBUG] ", message, ##__VA_ARGS__)

#elif LOGGING_TOP_LOG_LEVEL == LOG_INFO
#define LogCritical(message, ...) CRITICAL_LOG("[CRITICAL] ", message, ##__VA_ARGS__)
#define LogError(message, ...) LOG_WITH_FUNC("[ERROR] ", message, ##__VA_ARGS__)
#define LogWarn(message, ...) LOG_WITH_FUNC("[WARN]  ", message, ##__VA_ARGS__)
#define LogInfo(message, ...) LOG_WITH_FUNC("[INFO]  ", message, ##__VA_ARGS__)
#define LogDebug(message, ...)

#elif LOGGING_TOP_LOG_LEVEL == LOG_WARN
#define LogCritical(message, ...) CRITICAL_LOG("[CRITICAL] ", message, ##__VA_ARGS__)
#define LogError(message, ...) LOG_WITH_FUNC("[ERROR] ", message, ##__VA_ARGS__)
#define LogWarn(message, ...) LOG_WITH_FUNC("[WARN]  ", message, ##__VA_ARGS__)
#define LogInfo(message, ...)
#define LogDebug(message, ...)

#elif LOGGING_TOP_LOG_LEVEL == LOG_ERROR
#define LogCritical(message, ...) CRITICAL_LOG("[CRITICAL] ", message, ##__VA_ARGS__)
#define LogError(message, ...) LOG_WITH_FUNC("[ERROR] ", message, ##__VA_ARGS__)
#define LogWarn(message, ...)
#define LogInfo(message, ...)
#define LogDebug(message, ...)

#elif LOGGING_TOP_LOG_LEVEL == LOG_CRITICAL
#define LogCritical(message, ...) CRITICAL_LOG("[CRITICAL] ", message, ##__VA_ARGS__)
#define LogError(message, ...)
#define LogWarn(message, ...)
#define LogInfo(message, ...)
#define LogDebug(message, ...)

#else
#define LogCritical(message, ...)
#define LogError(message, ...)
#define LogWarn(message, ...)
#define LogInfo(message, ...)
#define LogDebug(message, ...)
#endif

#endif

#endif /* LOGGING_STACK_H */
