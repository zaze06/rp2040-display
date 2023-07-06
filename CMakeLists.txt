cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(display  C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(display main.c colors.h)

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
        )

add_subdirectory(./lib/Config)
add_subdirectory(./lib/LCD)
add_subdirectory(./lib/Fonts)
add_subdirectory(./lib/GUI)
add_subdirectory(./lib/QMI8658)

include_directories(./lib/Config)
include_directories(./lib/Fonts)
include_directories(./lib/GUI)
include_directories(./lib/LCD)
include_directories(./lib/QMI8658)

set(CMAKE_C_STANDARD 23)


# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(display pico_stdlib)

# create map/bin/hex/uf2 file in addition to ELF.

# enable usb output, disable uart output
pico_enable_stdio_usb(display 1)
pico_enable_stdio_uart(display 1)
pico_add_extra_outputs(display)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(display)

target_link_libraries(display LCD QMI8658 GUI Fonts Config pico_stdlib hardware_spi hardware_i2c hardware_rtc)