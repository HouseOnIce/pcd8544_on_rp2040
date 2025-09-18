cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(pcd8544_driver_example C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(pcd8544_on_rp2040
    main.c
    pcd8544.c
    pcd8544.h
    pcd8544_font.h
)

target_link_libraries(pcd8544_on_rp2040
    pico_stdlib
    hardware_spi
    hardware_gpio
)

pico_add_extra_outputs(pcd8544_on_rp2040)


