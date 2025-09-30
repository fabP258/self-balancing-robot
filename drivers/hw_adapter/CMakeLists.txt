add_library(hw_adapter
    pico_i2c_adapter.cpp
    pico_gpio_adapter.cpp
    pico_pwm_adapter.cpp
)

target_include_directories(hw_adapter PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(hw_adapter PUBLIC
    hardware_i2c
    hardware_gpio
    hardware_pwm
    driver_hal
)