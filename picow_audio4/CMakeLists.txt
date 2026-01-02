# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(picow_audio4 C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

###########################################################################################
include(FreeRTOS_Kernel_import.cmake)

# Add executable. Default name is the project name, version 0.1

# add_executable(pico_audio5 pico_audio5.c )

include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
)

file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/include/*.h"
        "${PROJECT_SOURCE_DIR}/include/*.hpp"
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/*.c"
        "${PROJECT_SOURCE_DIR}/src/*.s"
        )


add_executable(picow_audio4 ${all_SRCS})

# don't want NO_SYS (generally this would be in your lwipopts.h)
target_compile_definitions(picow_audio4 PRIVATE NO_SYS=0 )

# Add the standard include files to the build
target_include_directories(picow_audio4 PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  # ${CMAKE_CURRENT_LIST_DIR}/../.. # for our common lwipopts or any other standard includes, if required
)

#####################################################################################

pico_set_program_name(picow_audio4 "picow_audio4")
pico_set_program_version(picow_audio4 "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(picow_audio4 1)
pico_enable_stdio_usb(picow_audio4 0)


# Add any user requested libraries
target_link_libraries(picow_audio4 
        pico_stdlib
        hardware_spi
        hardware_timer
        hardware_clocks
        hardware_pwm
        pico_cyw43_arch_lwip_sys_freertos
        FreeRTOS-Kernel
        FreeRTOS-Kernel-Heap4
        tinyusb_device
)

pico_add_extra_outputs(picow_audio4)

