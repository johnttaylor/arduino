
# Arduino specific support for CPL
src/Bsp/Adafruit/feather52/gcc

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

# Sensor libraries
$ARDUINO_SKETCH_FOLDER$/libraries/Adafruit_BNO055
$ARDUINO_SKETCH_FOLDER$/libraries/Adafruit_Unified_Sensor

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
