# Quick Start Guide

## 🚀 Fastest Way to Run (5 minutes)

### Using FreeRTOS POSIX Simulator

1. **Clone FreeRTOS:**
   ```bash
   git clone https://github.com/FreeRTOS/FreeRTOS.git
   cd FreeRTOS/FreeRTOS/Demo/Posix_GCC
   ```

2. **Copy this project's files:**
   ```bash
   # Copy main.c from this project
   cp /path/to/freertos-task-monitoring/main.c ./
   
   # Update FreeRTOSConfig.h with settings from this project
   # Or merge the key config options:
   # - configUSE_TRACE_FACILITY = 1
   # - configUSE_STATS_FORMATTING_FUNCTIONS = 1
   # - configCHECK_FOR_STACK_OVERFLOW = 2
   # - configGENERATE_RUN_TIME_STATS = 1
   ```

3. **Build and run:**
   ```bash
   make
   ./demo
   ```

## 📋 Key Configuration Options

Add these to `FreeRTOSConfig.h`:

```c
#define configUSE_TRACE_FACILITY                1
#define configUSE_STATS_FORMATTING_FUNCTIONS    1
#define configCHECK_FOR_STACK_OVERFLOW          2
#define configGENERATE_RUN_TIME_STATS           1
#define INCLUDE_uxTaskGetStackHighWaterMark     1
#define INCLUDE_eTaskGetState                   1
```

## 🎯 What You'll See

The application will:
- Create 3 worker tasks with different stack sizes
- Create 1 monitoring task that reports every 5 seconds
- Show stack high-water marks for each task
- Flag tasks with low stack remaining (<200 bytes = WARNING, <100 bytes = CRITICAL)

## 📝 Resume Wording (Copy-Paste Ready)

**RTOS Task & Stack Monitoring Tool (FreeRTOS)**

* Implemented runtime task monitoring using FreeRTOS APIs to analyze stack usage, task states, and scheduling behavior in real-time systems
* Identified stack overflow risks and improved system safety through proactive memory usage analysis

## 🔧 Troubleshooting

**"FreeRTOS.h not found"**
- Ensure you're in the FreeRTOS demo directory
- Check that FreeRTOS source is cloned

**"undefined reference to vConfigureTimerForRunTimeStats"**
- Copy `runtime_stats.c` from this project
- Or implement these functions for your platform

**No output**
- Check that tasks are being created successfully
- Verify scheduler is starting (should see "Starting FreeRTOS scheduler...")

## 📚 Next Steps

- Integrate with ESP32 for hardware demonstration
- Add CSV logging for stack usage trends
- Implement alert system for critical stack conditions
- Add task CPU usage percentage tracking

