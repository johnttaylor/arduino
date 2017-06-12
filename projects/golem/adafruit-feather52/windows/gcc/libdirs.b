# My application
src/Golem
src/Golem/_minimalPolicies
src/Golem/_neoPixel
src/Golem/TShell/Cmd

# Arduino specific support for CPL
src/Bsp/Adafruit/feather52/gcc
src/Driver/Imu/Bno055
src/Imu/Motion/Cube
src/Cpl/TShell/Dac/Cmd/Arduino

# Colony::Core libraries (note: the Feather52/Arduino boards uses FreeRTOS)
//colony.core/src/Cpl/Io
//colony.core/src/Cpl/System/FreeRTOS
//colony.core/src/Cpl/System/FreeRTOS/_fatalerror
//colony.core/src/Cpl/System
//colony.core/src/Cpl/System/_trace
//colony.core/src/Cpl/System/_trace/_arduino
//colony.core/src/Cpl/Container
//colony.core/src/Cpl/Text
//colony.core/src/Cpl/Io/Serial/Adafruit/Nrf5

# DAC Shell (and infrastructure)
//colony.core/src/Cpl/TShell
//colony.core/src/Cpl/TShell/Dac
//colony.core/src/Cpl/TShell/Dac/Cmd
//colony.core/src/Cpl/TShell/Dac/Cmd/_basic
#//colony.core/src/Cpl/TShell/Dac/Cmd/_scripting
//colony.core/src/Cpl/TShell/Dac/Cmd/FreeRTOS
//colony.core/src/Cpl/Text/Frame
//colony.core/src/Cpl/Text/Tokenizer
//colony.core/src/Cpl/Memory
//colony.core/src/Cpl/Log
//colony.core/src/Cpl/Log/_default


# Sensor libraries
$ARDUINO_SKETCH_FOLDER$/libraries/Adafruit_BNO055
$ARDUINO_SKETCH_FOLDER$/libraries/Adafruit_Unified_Sensor
$ARDUINO_SKETCH_FOLDER$/libraries/Adafruit_NeoPixel

# Arduino Core library for the Nordic nrf52 Feather52
$ARDUINO_TOOLS$/hardware/nrf52/$ARDUINO_BSP_VER$/variants/feather52
$ARDUINO_TOOLS$/hardware/nrf52/$ARDUINO_BSP_VER$/cores/nRF5
$ARDUINO_TOOLS$/hardware/nrf52/$ARDUINO_BSP_VER$/cores/nRF5/avr
$ARDUINO_TOOLS$/hardware/nrf52/$ARDUINO_BSP_VER$/cores/nRF5/freertos/portable/CMSIS/nrf52
$ARDUINO_TOOLS$/hardware/nrf52/$ARDUINO_BSP_VER$/cores/nRF5/freertos/portable/GCC/nrf52
$ARDUINO_TOOLS$/hardware/nrf52/$ARDUINO_BSP_VER$/cores/nRF5/freertos/source
$ARDUINO_TOOLS$/hardware/nrf52/$ARDUINO_BSP_VER$/cores/nRF5/freertos/source/portable/MemMang
$ARDUINO_TOOLS$/hardware/nrf52/$ARDUINO_BSP_VER$/cores/nRF5/utility
$ARDUINO_TOOLS$/hardware/nrf52/$ARDUINO_BSP_VER$/cores/nRF5/SDK/components/toolchain
$ARDUINO_TOOLS$/hardware/nrf52/$ARDUINO_BSP_VER$/libraries/Wire
#$ARDUINO_TOOLS$/hardware/nrf52/$ARDUINO_BSP_VER$/libraries/Bluefruit52Lib/src
#$ARDUINO_TOOLS$/hardware/nrf52/$ARDUINO_BSP_VER$/libraries/Bluefruit52Lib/src/clients
#$ARDUINO_TOOLS$/hardware/nrf52/$ARDUINO_BSP_VER$/libraries/Bluefruit52Lib/src/services
