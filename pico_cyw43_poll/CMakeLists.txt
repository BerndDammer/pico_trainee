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

project(pico_llCyw43 C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

###########################################################################################
# add_compile_definitions(pico_llCyw43 PUBLIC CYW43_LWIP=FALSE )
# add_compile_definitions(pico_llCyw43 PUBLIC CYW43_SPI_PIO=1 ) # linking problem
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
        "${PROJECT_SOURCE_DIR}/src/*.S"
)


add_executable(pico_llCyw43 ${all_SRCS})

#####################################################################################

pico_set_program_name(pico_llCyw43 "pico__llCyw43")
pico_set_program_version(pico_llCyw43 "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(pico_llCyw43 1)
pico_enable_stdio_usb(pico_llCyw43 0)

# Add the standard library to the build
target_link_libraries(pico_llCyw43
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(pico_llCyw43 PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
# does not see pio functions cyw43_write_reg_ ...
target_link_libraries(pico_llCyw43 
        pico_cyw43_driver
        # pico_cyw43_arch_none
        pico_cyw43_arch_poll 
        pico_cyw43_arch 
        pico_async_context_poll 
        pico_lwip
        pico_lwip_nosys
)

pico_add_extra_outputs(pico_llCyw43)

