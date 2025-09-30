add_library(imu_driver
    imu_driver.cpp
    mpu6050_driver.cpp
)

target_include_directories(imu_driver PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(imu_driver PRIVATE driver_hal)