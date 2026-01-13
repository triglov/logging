/**
 * @file: logging.c
 * @author:: Paweł Kawula (pawel.kawula@kelectronics.pl)
 * -----
 * Copyright 2025 - KElectronics
 */

#include <stdarg.h>
#include <stddef.h>

#include "logging.h"
#include "logging_levels.h"

int (*log_function)(const char *message, ...) = NULL;
int (*critical_log_function)(const char *message, ...) = NULL;

static int default_log_function(const char *message, ...)
{
    (void)message;
    return 0;
}

void Logging_Init(Logging_Function_t log_func)
{
    if (log_func)
    {
        log_function = log_func;
    }
    else
    {
        log_function = default_log_function;
    }
}

void Logging_Init_Critical(Logging_Function_t log_func)
{
    if (log_func)
    {
        critical_log_function = log_func;
    }
    else
    {
        critical_log_function = default_log_function;
    }
}

const char *Logging_GetVersion(void)
{
    return LOGGING_VERSION;
}

char * Logging_GetLoggingLevelName(int level)
{
    switch (level)
    {
        case LOG_NONE:
            return "LOG_NONE";
        case LOG_CRITICAL:
            return "LOG_CRITICAL";
        case LOG_ERROR:
            return "LOG_ERROR";
        case LOG_WARN:
            return "LOG_WARN";
        case LOG_INFO:
            return "LOG_INFO";
        case LOG_DEBUG:
            return "LOG_DEBUG";
        default:
            return "UNKNOWN_LEVEL";
    }
}

int Logging_GetTopLoggingLevel(void)
{
    return LOGGING_TOP_LOG_LEVEL;
}