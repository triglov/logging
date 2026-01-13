/**
 * @file: logging_levels.h
 * @author: Paweł Kawula (pawel.kawula@kelectronics.pl)
 * @brief: Logging level definitions for embedded high-performance logging system
 * -----
 * Copyright 2025 - KElectronics
 * -----
 * @note: Core logging level concepts inspired by FreeRTOS logging framework,
 *        but implementation completely rewritten for embedded performance
 */

#ifndef LOGGING_LEVELS_H
#define LOGGING_LEVELS_H

/**
 * @brief Log levels for embedded high-performance logging system.
 *
 * Each module should specify a log level by setting @ref LOGGING_TOP_LOG_LEVEL.
 * All log messages with a level at or below the specified level will be printed
 * for that module with zero runtime overhead.
 *
 * Log levels in order of severity (lowest to highest):
 * - #LOG_NONE - Completely disable all logging
 * - #LOG_CRITICAL - Hard faults and unrecoverable system failures
 * - #LOG_ERROR - Critical failures requiring immediate attention  
 * - #LOG_WARN - Abnormal conditions that don't stop execution
 * - #LOG_INFO - Normal operational status messages
 * - #LOG_DEBUG - Detailed diagnostic information for development
 *
 * @note Higher log levels generate more output and impact performance.
 *       For production embedded systems, use LOG_CRITICAL or LOG_ERROR only.
 */

/**
 * @brief Disable all logging completely.
 * 
 * When LOGGING_TOP_LOG_LEVEL is LOG_NONE, all logging macros become empty
 * and generate zero code, providing maximum performance for production builds.
 */
#define LOG_NONE     0

/**
 * @brief Hard faults and unrecoverable system failures (highest severity).
 * 
 * Use for conditions that require immediate system intervention:
 * - Hard fault exceptions
 * - Stack overflow/underflow
 * - Watchdog resets
 * - Memory access violations
 * - Unrecoverable hardware failures
 * 
 * @note These messages should always be logged in production. Can be logged
 *       even with interrupts disabled via Logging_Init_Critical().
 */
#define LOG_CRITICAL 1

/**
 * @brief Critical system failures requiring immediate attention.
 * 
 * Use for conditions that prevent normal system operation:
 * - Hardware initialization failures
 * - Critical resource allocation failures  
 * - Security violations
 * - System reboots
 * 
 * @note These should be rare in production and require immediate attention.
 */
#define LOG_ERROR    2

/**
 * @brief Warning conditions that don't prevent operation.
 * 
 * Use for abnormal but recoverable conditions:
 * - Resource usage approaching limits
 * - Degraded performance conditions
 * - Retry operations
 * - Configuration issues with fallbacks
 * 
 * @note System continues operation but monitoring may be needed.
 */
#define LOG_WARN     3

/**
 * @brief Normal operational status messages.
 * 
 * Use for important system state changes:
 * - System startup/shutdown sequences
 * - Mode changes (sleep/wake, configuration changes)
 * - Connection status (network, peripherals)
 * - Major operation completions
 * 
 * @note Provides visibility into normal system behavior.
 */
#define LOG_INFO     4

/**
 * @brief Detailed diagnostic information for development.
 * 
 * Use for fine-grained debugging information:
 * - Function entry/exit with parameters
 * - Internal state variables
 * - Buffer contents and data flows
 * - Performance timing measurements
 * 
 * @warning Should be disabled in production builds due to performance impact.
 */
#define LOG_DEBUG    5

#endif /* ifndef LOGGING_LEVELS_H */
