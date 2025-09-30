add_library(h_bridge_driver
    h_bridge_driver_gravity.cpp
)

target_include_directories(h_bridge_driver PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(h_bridge_driver PUBLIC
    driver_hal
)