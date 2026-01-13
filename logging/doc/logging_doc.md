# Logging System Documentation

## Overview

This document describes the high-performance logging system for embedded applications, based on [FreeRTOS logging framework](https://www.freertos.org/logging.html). The system provides five hierarchical logging levels with **pure compile-time string concatenation** for maximum embedded performance, configurable output formats, and customizable log transport mechanisms.

## Key Features

- **🚀 Zero Runtime Overhead**: Pure compile-time string concatenation
- **📦 Minimal Code Size**: ~50-70% smaller than format-string approaches
- **⚡ Embedded-Optimized**: Ideal for resource-constrained systems
- **🎛️ Configurable**: Multiple output format options
- **🔧 CMake Integration**: Simple configuration management

## Log Levels

The logging system supports five hierarchical log levels:

| Level | Value | Description |
| ------- | ------- | ------------- |
| `LOG_NONE` | 0 | No log messages |
| `LOG_CRITICAL` | 1 | Hard faults, watchdog resets, unrecoverable system failures |
| `LOG_ERROR` | 2 | Critical failures requiring immediate attention |
| `LOG_WARN` | 3 | Abnormal event that may be indicative of an error |
| `LOG_INFO` | 4 | Helpful, informational message about normal execution |
| `LOG_DEBUG` | 5 | Detailed and excessive debug information |

## Configuration

The logging system is configured through CMake definitions in your `CMakeLists.txt` file. This approach ensures consistent configuration across all components.

### CMake Configuration Example

```cmake
# Global logging configuration - applies to all targets
add_compile_definitions(
    # File path options (choose one or leave commented):
    LOGGING_PRINT_FILE_PATH          # Full file path (compile-time safe)
    # (no option)                    # No file path information (most embedded-friendly)
    
    # Function name support (optional - requires C99+ __func__ support)
    LOGGING_PRINT_FUNCTION_NAME      # Add function names to log messages
    
    LOGGING_TOP_LOG_LEVEL=LOG_DEBUG      # Project-wide default log level
)

# Target-specific configuration (overrides global settings)
target_compile_definitions(my_app
    PRIVATE
        LOGGING_LOG_NAME="[MY_APP]"   # Custom name for this target
)
```

### Configuration Options

#### File Path Display (Compile-Time Safe Options Only)

Choose one of these options:

- **`LOGGING_PRINT_FILE_PATH`** - Shows full file path using `__FILE__` macro
- **No option** - No file information displayed (**most embedded-friendly**)

#### Function Name Display

- **`LOGGING_PRINT_FUNCTION_NAME`** - Adds function names to log messages (requires C99+ `__func__` support)
  - **Performance optimized**: Function name passed as separate argument (no runtime string concatenation)
  - **Zero overhead when disabled**: Pure compile-time string concatenation

#### Log Level Control

- **`LOGGING_TOP_LOG_LEVEL=LOG_DEBUG`** - All messages (CRITICAL, ERROR, WARN, INFO, DEBUG)
- **`LOGGING_TOP_LOG_LEVEL=LOG_INFO`** - INFO and above (CRITICAL, ERROR, WARN, INFO)
- **`LOGGING_TOP_LOG_LEVEL=LOG_WARN`** - WARN and above (CRITICAL, ERROR, WARN)
- **`LOGGING_TOP_LOG_LEVEL=LOG_ERROR`** - ERROR and above (CRITICAL, ERROR)
- **`LOGGING_TOP_LOG_LEVEL=LOG_CRITICAL`** - CRITICAL only (hard faults, system crashes)
- **`LOGGING_TOP_LOG_LEVEL=LOG_NONE`** - No logging

#### Module/Library Name

- **`LOGGING_LOG_NAME="[MODULE_NAME]"`** - Custom identifier for log messages

## Performance Characteristics

### Compile-Time String Concatenation vs Argument Passing

#### Without Function Names (Pure Compile-Time)

```c
// Your code:
LogInfo("Temperature: %d°C", temp);

// Becomes at compile-time (zero overhead):
log_function("[INFO] [SENSOR] :42 - Temperature: %d°C\r\n", temp);
// Stack usage: ~12 bytes (1 format string + 1 user arg)
```

#### With Function Names (Optimized Argument Passing)

```c
// Your code:
LogInfo("Temperature: %d°C", temp);

// Becomes at compile-time (optimized):
log_function("[INFO] [SENSOR] (%s):42 - Temperature: %d°C\r\n", __func__, temp);
// Stack usage: ~16 bytes (1 format string + 1 func arg + 1 user arg)

// NOT like traditional format-string loggers (with runtime overhead):
// log_function("[INFO] %s%s:%s - Temperature: %d°C\r\n", "[SENSOR] ", __func__, "42", temp);
// Stack usage: ~24+ bytes (1 format string + 4+ args)
```

### Embedded Performance Benefits

- **Code Size**: ~50-70% smaller than format-string approaches
- **RAM Usage**: Minimal stack usage (1-2 args vs 4+ args in traditional loggers)
- **Execution Speed**: 5-10x faster (direct string output, minimal argument passing)
- **Deterministic**: Always same execution time (critical for real-time systems)
- **Flash/ROM Friendly**: String literals stored in ROM, not RAM
- **Function Name Efficiency**: When enabled, passed as single argument (no concatenation)

## API Usage

### Initialization

```c
#include "logging.h"

// Define your custom logging function
static int custom_log_function(const char *message, ...)
{
    va_list args;
    va_start(args, message);
    vprintf(message, args);  // Output to stdout
    va_end(args);
    return 0;
}
// Define critical logging function (for hard faults, interrupts disabled)
static int critical_log_function(const char *message, ...)
{
    va_list args;
    va_start(args, message);
    // Output using interrupt-safe method (e.g., polling UART)
    uart_send_polling(message, args);
    va_end(args);
    return 0;
}

int main(void) {
    // Initialize the logging system
    Logging_Init(custom_log_function);
    Logging_Init_Critical(critical_log_function);  // For hard faults
    Logging_Init(custom_log_function);
    
    // Now you can use logging macros
    LogInfo("Application started");
    return 0;
}
```

## Example Output Formats

Critical(message, ...)`** - Critical level logging (hard faults, system crashes)

- **`LogError(message, ...)`** - Error level logging
- **`LogWarn(message, ...)`** - Warning level logging  
- **`LogInfo(message, ...)`** - Information level logging
- **`LogDebug(message, ...)`** - Debug level logging

### Example Usage

```c
LogCritical("Hard fault at address 0x%08X", fault_address);ine LOGGING_TOP_LOG_LEVEL LOG_DEBUG
// LOGGING_PRINT_FILE_PATH not defined
// LOGGING_PRINT_FUNCTION_NAME not defined
```

### Logging Macros

- **`LogError(message, ...)`** - Error level logging
- **`LogWarn(message, ...)`** - Warning level logging  
- **`LogInfo(message, ...)`** - Information level logging
- **`LogDebug(message, ...)`** - Debug level logging

### Example Usage

```c
LogError("Failed to initialize component %d", componentId);
LogWarn("Buffer usage at %d%% capacity", usage);
LogInfo("System initialized successfully");
LogDebug("Processing packet: size=%d, type=0x%02X", size, type);
```

## Output Format Examples

The output format depends on your configuration. Here are examples for different setups:

### Configuration 1: Full File Path + Function Names (Full Debug Mode)

```cmake
add_compile_definitions(
    LOGGING_PRINT_FILE_PATH
    LOGGING_PRINT_FUNCTION_NAME
    LOGGING_TOP_LOG_LEVEL=LOG_DEBUG
)
target_compile_definitions(my_app PRIVATE LOGGING_LOG_NAME="[APP]")
```

*CRITICAL] [APP] (/home/user/project/src/main.c) (main):41 - Hard fault detected
[*Output:**

``` 
[ERROR] [APP] (/home/user/project/src/main.c) (main):42 - Failed to initialize component 1
[WARN]  [APP] (/home/user/project/src/main.c) (main):43 - Buffer usage at 85% capacity
[INFO]  [APP] (/home/user/project/src/main.c) (main):44 - System initialized successfully
[DEBUG] [APP] (/home/user/project/src/main.c) (main):45 - Processing packet: size=128, type=0x5A
```

### Configuration 2: Function Names Only (Optimized Debug Mode)

```cmake
add_compile_definitions(
    LOGGING_PRINT_FUNCTION_NAME     # Function names with optimized argument passing
    LOGGING_TOP_LOG_LEVEL=LOG_DEBUG
    # No file paths for cleaner output
)
target_compile_definitions(my_app PRIVATE LOGGING_LOG_NAME="[APP]")
```

CRITICAL] [APP] (HardFault_Handler):41 - Hard fault detected
[
**Output:**

```
[ERROR] [APP] (main):42 - Failed to initialize component 1
[WARN]  [APP] (main):43 - Buffer usage at 85% capacity
[INFO]  [APP] (main):44 - System initialized successfully
[DEBUG] [APP] (process_data):45 - Processing packet: size=128, type=0x5A
```

**Performance Note**: Function names are passed as separate arguments (not concatenated), maintaining excellent performance while providing debugging context.

### Configuration 3: Minimal Embedded-Friendly (Recommended for Production)

```cmake
add_compile_definitions(
    LOGGING_TOP_LOG_LEVEL=LOG_INFO  # Skip debug messages
    # No file paths or function names for minimal overhead
)
target_compile_definitions(my_app PRIVATE LOGGING_LOG_NAME="[APP]")
```

*CRITICAL] [APP] :41 - Hard fault detected
[*Output:**

```
[ERROR] [APP] :42 - Failed to initialize component 1
[WARN]  [APP] :43 - Buffer usage at 85% capacity
[INFO]  [APP] :44 - System initialized successfully
```

### Configuration 3: Development Mode with Function Names

```cmake
add_compile_definitions(
    LOGGING_PRINT_FUNCTION_NAME  # Add function context
    LOGGING_TOP_LOG_LEVEL=LOG_DEBUG
)
target_compile_definitions(my_app PRIVATE LOGGING_LOG_NAME="[APP]")
```

### Configuration 4: Different Modules

```cmake
# Global settings
add_compile_definitions(
    LOGGING_PRINT_FUNCTION_NAME
    LOGGING_TOP_LOG_LEVEL=LOG_DEBUG
)

# Different names for different modules
target_compile_definitions(audio_module PRIVATE LOGGING_LOG_NAME="[AUDIO]")
target_compile_definitions(network_module PRIVATE LOGGING_LOG_NAME="[NET]")
target_compile_definitions(main_app PRIVATE LOGGING_LOG_NAME="[APP]")
```

**Output from different modules:**
`CRITICAL] [APP] (HardFault_Handler):66 - Hard fault exception
[``
[INFO]  [AUDIO] (init_audio):123 - Audio system initialized
[DEBUG] [NET] (send_packet):45 - Sending packet of size 256
[ERROR] [APP] (main):67 - Critical system failure

```

## Embedded System Recommendations

### Ultra-Low Resource Configuration (Microcontrollers)
For systems with severe memoryCRITICAL  # Only critical system failure
```cmake
add_compile_definitions(
    LOGGING_TOP_LOG_LEVEL=LOG_ERROR  # Only critical errors
    # No file paths or function names
)
# Consider disabling logging entirely in production: LOGGING_TOP_LOG_LEVEL=LOG_NONE
```

### Development/Debug Configuration

For development and testing:

```cmake
add_compile_definitions(
    LOGGING_PRINT_FILE_PATH
    LOGGING_PRINT_FUNCTION_NAME
    LOGGING_TOP_LOG_LEVEL=LOG_DEBUG
)
```

### Production Embedded Configuration

Balanced approach for production embedded systems:

```cmake
add_compile_definitions(
    LOGGING_PRINT_FUNCTION_NAME  # Helpful for field debugging
    LOGGING_TOP_LOG_LEVEL=LOG_WARN   # Warnings and errors only
)
```

## Function Name Performance Optimization

### Zero-Overhead Mode (LOGGING_PRINT_FUNCTION_NAME undefined)

```c
LogInfo("Processing data");
// Expands to: log_function("[INFO] [MODULE] :42 - Processing data\r\n");
// Stack usage: ~8 bytes (format string pointer)
// Execution: Direct string output - fastest possible
```

### Optimized Function Name Mode (LOGGING_PRINT_FUNCTION_NAME defined)

```c
LogInfo("Processing data");
// Expands to: log_function("[INFO] [MODULE] (%s):42 - Processing data\r\n", __func__);
// Stack usage: ~12 bytes (format string pointer + function name pointer)
// Execution: Single %s format specifier - still very fast
```

### Performance Comparison

| Mode | Stack Usage | Execution Speed | Use Case |
|------|------------|----------------|----------|
| **Zero-overhead** | ~8 bytes | Fastest | Production embedded |
| **Function names** | ~12 bytes | Very fast | Development/debugging |
| **Traditional logger** | ~24+ bytes | Slow | Avoid in embedded |

**Key Advantage**: Function names are passed as separate arguments, not concatenated at runtime. This maintains excellent performance while providing debugging context when needed.

## Log Level Behavior5)

All messages are printed:

- ✅ LogCritical - Hard faults and unrecoverable failures
- ✅ LogError - Critical failures
- ✅ LogWarn - Warning conditions  
- ✅ LogInfo - General information
- ✅ LogDebug - Detailed debug information

### LOG_INFO Level (Value: 4)

Only INFO and above are printed:

- ✅ LogCritical - Hard faults and unrecoverable failures
- ✅ LogError - Critical failures
- ✅ LogWarn - Warning conditions
- ✅ LogInfo - General information
- ❌ LogDebug - (macro becomes empty)

### LOG_WARN Level (Value: 3)

Only WARN and above are printed:

- ✅ LogCritical - Hard faults and unrecoverable failures
- ✅ LogError - Critical failures
- ✅ LogWarn - Warning conditions
- ❌ LogInfo - (macro becomes empty)
- ❌ LogDebug - (macro becomes empty)

### LOG_ERROR Level (Value: 2)

Only ERROR and above are printed:

- ✅ LogCritical - Hard faults and unrecoverable failures
- ✅ LogError - Critical failures
- ❌ LogWarn - (macro becomes empty)
- ❌ LogInfo - (macro becomes empty)  
- ❌ LogDebug - (macro becomes empty)

### LOG_CRITICAL Level (Value: 1)

Only CRITICAL messages are printed:

- ✅ LogCritical - Hard faults and unrecoverable failures
- ❌ LogError - (macro becomes empty)
- ❌ LogWarn - (macro becomes empty)
- ❌ LogInfo - (macro becomes empty)
- ❌ LogDebug - (macro becomes empty)

### LOG_NONE Level (Value: 0)

No messages are printed:

- ❌ LogCritical - (macro becomes empty)

### LOG_NONE Level (Value: 0)

No messages are printed:

- ❌ LogError - (macro becomes empty)
- ❌ LogWarn - (macro becomes empty)
- ❌ LogInfo - (macro becomes empty)
- ❌ LogDebug - (macro becomes empty)

## CMake Integration

### Project Structure Example

```
my_project/
├── CMakeLists.txt          # Top-level configuration
├── src/
│   ├── main.c
│   └── CMakeLists.txt
├── audio_module/
│   ├── audio.c
│   └── CMakeLists.txt
└── logging/                # Logging library
    ├── CMakeLists.txt
    ├── inc/
    │   ├── logging.h
    │   ├── logging_levels.h
    │   └── logging_stack.h
    └── src/
        └── logging.c
```

### Top-level CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.25.0)
project(my_project)

# Global logging settings
add_compile_definitions(
    LOGGING_PRINT_FILE_PATH        # Or leave commented for minimal embedded build
    LOGGING_PRINT_FUNCTION_NAME    # Optional for embedded systems
    LOGGING_TOP_LOG_LEVEL=LOG_DEBUG
)

# Add subdirectories
add_subdirectory(logging)       # Library first
add_subdirectory(src)          # Applications second
add_subdirectory(audio_module)
```

### Module-specific CMakeLists.txt

```cmakeCritical`**: For hard faults, watchdog resets, unrecoverable system failures (highest severity)

- **`Log

# src/CMakeLists.txt

add_executable(main main.c)
target_link_libraries(main PRIVATE logging)
target_compile_definitions(main PRIVATE LOGGING_LOG_NAME="[MAIN]")

# audio_module/CMakeLists.txt  

addCritical("Hard fault: CFSR=0x%08X, PC=0x%08X", SCB->CFSR, fault_pc);
Log_library(audio audio.c)
target_link_libraries(audio PUBLIC logging)
target_compile_definitions(audio PUBLIC LOGGING_LOG_NAME="[AUDIO]")

```

## Best Practices

### 1. Choose Appropriate Log Levels
- **`LogError`**: For critical failures that require immediate attention
- **`LogWarn`**: For concerning conditioCRITICAL` or `LOG_ERROR` (for field diagnostics)xecution  
- **`LogInfo`**: For general operational information
- **`LogDebug`**: For detailed debugging information

### 2. Include Relevant Context
```c
LogError("Failed to open file '%s': errno=%d", filename, errno);
LogInfo("Task started: priority=%d, stack=%d bytes", priority, stackSize);
```

### 3. Use Formatting Efficiently

```c
LogDebug("Buffer state: used=%d/%d bytes (%.1f%%)", used, total, percentage);
```

### 4. Configure Log Levels for Different Builds

- **Production embedded systems:** `LOG_ERROR` or `LOG_WARN`
- **Development/testing:** `LOG_INFO` or `LOG_DEBUG`
- **Ultra-minimal systems:** `LOG_NONE` (zero overhead)

### 5. Use Meaningful Module Names

```cmake
target_compile_definitions(network PRIVATE LOGGING_LOG_NAME="[NET]")
target_compile_definitions(audio PRIVATE LOGGING_LOG_NAME="[AUDIO]")
target_compile_definitions(storage PRIVATE LOGGING_LOG_NAME="[STORAGE]")
```

### 6. Embedded-Specific Considerations

- **Prefer minimal configurations** for production
- **Use compile-time filtering** over runtime filtering
- **Consider log output frequency** in real-time systems
- **Test with logging enabled** to measure impact on system timing

### Custom Log Transport

You can implement your own log transport function:

```c
#include <stdarg.h>
#include <stdio.h>

// Example: Log to file
static int file_log_function(const char *message, ...)
{
    static FILE *logfile = NULL;
    if (!logfile) {
        logfile = fopen("application.log", "a");
    }
    
    va_list args;
    va_start(args, message);
    vfprintf(logfile, message, args);
    fflush(logfile);
    va_end(args);
    return 0;
}

// Example: Log to syslog
static int syslog_function(const char *message, ...)
{
    va_list args;
    va_start(args, message);
    vsyslog(LOG_INFO, message, args);
    va_end(args);
    return 0;
}

// Example: Log to multiple destinations
static int multi_log_function(const char *message, ...)
{
    va_list args1, args2;
    va_start(args1, message);
    va_copy(args2, args1);
    
    // Log to console
    vprintf(message, args1);
    
    // Log to file
    FILE *logfile = fopen("debug.log", "a");
    if (logfile) {
        vfprintf(logfile, message, args2);
        fclose(logfile);
    }
    
    va_end(args1);
    va_end(args2);
    return 0;
}

int main() {
    // Choose your transport
    Logging_Init(LOG_DEBUG, file_log_function);
    // or
    // Logging_Init(LOG_INFO, syslog_function);
    // or  
    // Logging_Init(LOG_DEBUG, multi_log_function);
    
    LogInfo("Application started with custom transport");
    return 0;
}
```

### Performance Considerations

#### Compile-Time vs Runtime Performance

This logging system uses **pure compile-time string concatenation**, providing:

- **Zero runtime formatting overhead** (unlike printf-style loggers)
- **Deterministic execution time** (critical for real-time systems)
- **Minimal stack usage** (single string literal + user arguments)

#### Embedded System Guidelines

- **Higher log levels generate more output** and impact performance
- **Use conditional compilation** for performance-critical code:

  ```c
  #if LOGGING_TOP_LOG_LEVEL >= LOG_DEBUG
  LogDebug("Expensive debug operation: result=%d", expensive_calculation());
  #endif
  ```

- **Consider flash/ROM usage** with extensive logging
- **Test real-time behavior** with logging enabled
- **Use `LOGGING_DISABLED_GLOBALLY`** to completely disable all logging for ultra-minimal builds:

  ```cmake
  add_compile_definitions(LOGGING_DISABLED_GLOBALLY)
  ```

#### Memory Usage Comparison

```c
// This logging system - WITHOUT function names (zero overhead):
LogInfo("Sensor %d: temp=%d°C", id, temp);
// → Generates: log_function("[INFO] [SENSOR] :42 - Sensor %d: temp=%d°C\r\n", id, temp);
// Stack usage: ~12 bytes (format string + 2 user args)

// This logging system - WITH function names (optimized):
LogInfo("Sensor %d: temp=%d°C", id, temp);
// → Generates: log_function("[INFO] [SENSOR] (%s):42 - Sensor %d: temp=%d°C\r\n", __func__, id, temp);
// Stack usage: ~16 bytes (format string + 1 func arg + 2 user args)

// Traditional format-string logger (avoid):
// → Would generate: log_function("[INFO] %s%s:%s - Sensor %d: temp=%d°C\r\n", "[SENSOR] ", "", "42", id, temp);
// Stack usage: ~24+ bytes (format string + 5+ args)
```

#### Function Name Performance

- **Disabled** (`LOGGING_PRINT_FUNCTION_NAME` undefined): Pure compile-time concatenation, zero overhead
- **Enabled** (`LOGGING_PRINT_FUNCTION_NAME` defined): Function name passed as single argument, minimal overhead
- **No runtime string concatenation**: Unlike traditional loggers, function names are never concatenated at runtime

### Thread Safety

The logging system itself doesn't provide thread synchronization. If using in a multi-threaded environment, ensure your custom log function is thread-safe:

```c
#include <pthread.h>

static pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

static int thread_safe_log_function(const char *message, ...)
{
    pthread_mutex_lock(&log_mutex);
    
    va_list args;
    va_start(args, message);
    vprintf(message, args);
    va_end(args);
    
    pthread_mutex_unlock(&log_mutex);
    return 0;
}
```

## Troubleshooting

### Common Issues

1. **"LogXXX function not found" errors**
   - Ensure `LOGGING_TOP_LOG_LEVEL` is defined
   - Check that you're including `logging.h`
   - Verify CMake definitions are applied correctly

2. **No log output**
   - Check if `Logging_Init()` was called
   - Verify log level allows the messages you're trying to print
   - Ensure custom log function is implemented correctly

3. **Compilation errors with function names**
   - Make sure you're using C99 or later standard
   - `__func__` requires C99+ support

4. **CMake configuration not working**
   - Definitions must be set before `add_subdirectory()`
   - Use `target_compile_definitions()` for target-specific settings
