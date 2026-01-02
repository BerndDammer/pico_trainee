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
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(pico_usb_audio3 C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

###########################################################################################

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


add_executable(pico_usb_audio3 ${all_SRCS})

# don't want NO_SYS (generally this would be in your lwipopts.h)
target_compile_definitions(pico_usb_audio3 PRIVATE NO_SYS=0 )

# Add the standard include files to the build
target_include_directories(pico_usb_audio3 PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
)

#####################################################################################
target_compile_definitions(pico_usb_audio3 PRIVATE
            AUDIO_FREQ_MAX=48000

            # ours are zero based, so say so
            PICO_USBDEV_USE_ZERO_BASED_INTERFACES=0

            # need large descriptor
            PICO_USBDEV_MAX_DESCRIPTOR_SIZE=256


            PICO_USBDEV_ISOCHRONOUS_BUFFER_STRIDE_TYPE=1
            PICO_USBDEV_ENABLE_DEBUG_TRAgCE

            PICO_AUDIO_I2S_MONO_OUTPUT=0
            PICO_AUDIO_I2S_MONO_INPUT=0
)
########################################################################################

pico_set_program_name(pico_usb_audio3 "pico_usb_audio3")
pico_set_program_version(pico_usb_audio3 "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(pico_usb_audio3 0)
pico_enable_stdio_usb(pico_usb_audio3 0)

# Add the standard library to the build
target_link_libraries(pico_usb_audio3
    pico_stdlib
    hardware_irq
    pico_fix_rp2040_usb_device_enumeration
    pico_sync
    pico_stdio_uart
)

pico_add_extra_outputs(pico_usb_audio3)

