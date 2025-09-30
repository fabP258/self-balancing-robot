add_library(control
    estimation/complementary_filter.cpp
    control/pid_controller.cpp
)

target_include_directories(control PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/estimation
    ${CMAKE_CURRENT_LIST_DIR}/control
)