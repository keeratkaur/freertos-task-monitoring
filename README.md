# FreeRTOS Task Monitoring & Stack Usage Analyzer

A real-time system monitoring tool that analyzes task behavior, stack usage, and detects potential stack overflow conditions in FreeRTOS-based applications.

## What This Project Demonstrates

This project showcases **systems-level thinking** by implementing:

- **Real-time task monitoring** using FreeRTOS introspection APIs
- **Stack usage analysis** to identify memory safety risks
- **Stack overflow detection** through proactive monitoring
- **Task state tracking** to understand RTOS scheduling behavior

## Why This Matters

In embedded systems and real-time operating systems, understanding task behavior is critical for:

- **Memory Safety**: Stack overflows can cause unpredictable system crashes
- **System Reliability**: Monitoring task states helps identify deadlocks and priority inversions
- **Resource Management**: Stack high-water marks reveal memory efficiency
- **Debugging**: Runtime statistics provide insights into system behavior

This directly maps to **OS/RTOS internals** and demonstrates understanding of:
- Task scheduling and preemption
- Stack vs heap memory management
- System introspection and observability
- Safety-first design principles

## Architecture

The application creates multiple worker tasks with different stack sizes and work patterns, then uses a dedicated monitoring task to periodically analyze:

1. **Task States**: Running, Ready, Blocked, Suspended
2. **Stack High-Water Marks**: Remaining stack space for each task
3. **Task Priorities**: Current priority levels
4. **Runtime Statistics**: Total system runtime

The monitor identifies tasks with low stack remaining and flags them as warnings or critical risks.

## Features

- ✅ Real-time task state monitoring
- ✅ Stack usage tracking with high-water mark analysis
- ✅ Stack overflow risk detection (warnings for <200 bytes, critical for <100 bytes)
- ✅ Runtime statistics collection
- ✅ Multiple task demonstration with varying stack sizes
- ✅ Stack overflow hook implementation

## Configuration

Key FreeRTOS configuration options enabled in `FreeRTOSConfig.h`:

```c
#define configUSE_TRACE_FACILITY                1
#define configUSE_STATS_FORMATTING_FUNCTIONS    1
#define configCHECK_FOR_STACK_OVERFLOW          2
#define configGENERATE_RUN_TIME_STATS           1
```

## Building and Running

### Prerequisites

- GCC compiler
- FreeRTOS kernel source (or POSIX simulator)
- Make

### Build Instructions

```bash
# Clone or download FreeRTOS source
# Place FreeRTOS kernel files in appropriate directories
# Or use FreeRTOS POSIX simulator

make
./freertos_monitor
```

### Using FreeRTOS POSIX Simulator

For the simplest setup, use the [FreeRTOS POSIX Simulator](https://github.com/FreeRTOS/FreeRTOS/tree/main/FreeRTOS/Demo/Posix_GCC):

1. Clone FreeRTOS repository
2. Copy this project's `main.c` to the POSIX demo directory
3. Update `FreeRTOSConfig.h` as needed
4. Build using the provided Makefile

## Sample Output

```
========================================
FreeRTOS Task Monitoring & Stack Analyzer
========================================

--- Task Status Report ---
Task Name            | State        | Stack HW Mark   | Priority  
------------------------------------------------------------
MonitorTask          | Running      | 1800            | 3         
WorkerTask1          | Ready        | 850             | 2         
WorkerTask2          | Blocked      | 920             | 2         
SmallStackTask       | Ready        | 180             | 2         [WARNING]

Total Runtime: 50000 ticks
Total Tasks: 4
```

## 🔍 Key Implementation Details

### Stack Monitoring

The monitoring task uses `uxTaskGetSystemState()` to retrieve:
- Task status structures for all tasks
- Stack high-water marks (minimum remaining stack)
- Current task states
- Priority information

### Stack Overflow Detection

FreeRTOS is configured with `configCHECK_FOR_STACK_OVERFLOW 2`, which:
- Uses a pattern-based detection method
- Calls `vApplicationStackOverflowHook()` on overflow
- Provides task handle and name for debugging

### Memory Safety

The monitor allocates memory for task status arrays using `pvPortMalloc()`, demonstrating proper heap usage in an RTOS environment.

## Technical Highlights

- **RTOS Internals**: Direct use of FreeRTOS kernel APIs for system introspection
- **Memory Management**: Stack vs heap awareness, proper allocation patterns
- **Safety Engineering**: Proactive overflow detection and risk assessment
- **System Observability**: Real-time monitoring without external debuggers

## Learning Outcomes

This project demonstrates understanding of:

1. **Task Scheduling**: How RTOS manages multiple concurrent tasks
2. **Stack Management**: Why stack monitoring is critical in embedded systems
3. **System Introspection**: Using OS APIs to analyze runtime behavior
4. **Safety Patterns**: Detecting and preventing memory-related failures

## Related Concepts

- Real-Time Operating Systems (RTOS)
- Task scheduling and preemption
- Stack overflow protection
- Memory safety in embedded systems
- System monitoring and observability

## License

This project is provided as a demonstration of FreeRTOS monitoring capabilities. FreeRTOS itself is licensed under the MIT license.

## Acknowledgments

Built using [FreeRTOS](https://www.freertos.org/) - a market-leading real-time operating system kernel.

---

**Note**: This is a demonstration project focusing on RTOS internals and system monitoring. For production use, additional error handling, logging infrastructure, and testing would be recommended.

