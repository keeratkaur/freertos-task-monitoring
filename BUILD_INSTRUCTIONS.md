# Build Instructions

This project demonstrates FreeRTOS task monitoring. To build and run it, you have two main options:

## Option 1: FreeRTOS POSIX Simulator (Recommended - Easiest)

The POSIX simulator allows you to run FreeRTOS on your development machine without hardware.

### Steps:

1. **Clone FreeRTOS repository:**
   ```bash
   git clone https://github.com/FreeRTOS/FreeRTOS.git
   ```

2. **Copy project files:**
   - Copy `main.c` from this project
   - Copy or merge `FreeRTOSConfig.h` settings
   - Place files in the POSIX demo directory structure

3. **Build using FreeRTOS's provided Makefile:**
   ```bash
   cd FreeRTOS/FreeRTOS/Demo/Posix_GCC
   # Copy main.c here
   make
   ./demo
   ```

### Alternative: Standalone Build

If you want to build this project standalone:

1. **Set up FreeRTOS source:**
   ```bash
   # Clone FreeRTOS
   git clone https://github.com/FreeRTOS/FreeRTOS.git
   
   # Or download and extract to ./FreeRTOS directory
   ```

2. **Update Makefile paths:**
   - Edit `Makefile` and adjust `FREERTOS_DIR` if needed

3. **Build:**
   ```bash
   make
   ./freertos_monitor
   ```

## Option 2: ESP32 (Hardware Target)

For ESP32, use the official FreeRTOS ESP32 demo:

1. **Clone FreeRTOS ESP32 demo:**
   ```bash
   git clone https://github.com/FreeRTOS/FreeRTOS.git
   cd FreeRTOS/FreeRTOS/Demo/ESP32
   ```

2. **Integrate monitoring code:**
   - Add `vMonitorTask` from `main.c` to your ESP32 project
   - Enable required config options in `FreeRTOSConfig.h`
   - Build using ESP-IDF build system

3. **Flash and monitor:**
   ```bash
   idf.py build flash monitor
   ```

## Required FreeRTOS Configuration

Ensure these are enabled in `FreeRTOSConfig.h`:

```c
#define configUSE_TRACE_FACILITY                1
#define configUSE_STATS_FORMATTING_FUNCTIONS    1
#define configCHECK_FOR_STACK_OVERFLOW          2
#define configGENERATE_RUN_TIME_STATS           1
#define INCLUDE_uxTaskGetStackHighWaterMark     1
#define INCLUDE_eTaskGetState                   1
```

## Dependencies

- **GCC compiler** (for POSIX simulator)
- **FreeRTOS kernel source** (v10.4.3 or compatible)
- **pthread library** (for POSIX port)
- **Make** (for building)

## Troubleshooting

### "FreeRTOS.h: No such file or directory"
- Ensure FreeRTOS source is in the correct location
- Check `INCLUDES` paths in Makefile

### "undefined reference to vConfigureTimerForRunTimeStats"
- Implement this function for your target platform
- For POSIX simulator, it's typically provided in the port layer

### Stack overflow hook not called
- Verify `configCHECK_FOR_STACK_OVERFLOW` is set to 2
- Ensure `vApplicationStackOverflowHook` is implemented

## Quick Test (POSIX Simulator)

The fastest way to see this in action:

```bash
# Use the official FreeRTOS POSIX demo as base
git clone https://github.com/FreeRTOS/FreeRTOS.git
cd FreeRTOS/FreeRTOS/Demo/Posix_GCC

# Replace main.c with this project's main.c
# Update FreeRTOSConfig.h with required settings
make
./demo
```

You should see periodic task monitoring output showing stack usage and task states.

