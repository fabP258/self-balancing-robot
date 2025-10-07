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
set(picotoolVersion 2.2.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2 CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(self-balancing-robot C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

add_compile_options(-Wall -Wextra -Werror)

# Add executable. Default name is the project name, version 0.1

add_executable(self-balancing-robot self-balancing-robot.cpp src/app.cpp)

pico_set_program_name(self-balancing-robot "self-balancing-robot")
pico_set_program_version(self-balancing-robot "0.1")

# Modify the below lines to enable/disable output over UART/USB
# pico_enable_stdio_uart(self-balancing-robot 0)
pico_enable_stdio_usb(self-balancing-robot 1)

add_subdirectory(drivers/imu)
add_subdirectory(drivers/hw_adapter)
add_subdirectory(drivers/hal)
add_subdirectory(drivers/h_bridge)
add_subdirectory(src/control)

# Add the standard library to the build
target_link_libraries(self-balancing-robot
        pico_stdlib
        hardware_i2c
        hardware_timer
        driver_hal
        imu_driver
        h_bridge_driver
        hw_adapter
        control
)

# Add the standard include files to the build
target_include_directories(self-balancing-robot PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        src
)

pico_add_extra_outputs(self-balancing-robot)

