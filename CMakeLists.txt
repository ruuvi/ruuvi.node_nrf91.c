cmake_minimum_required(VERSION 3.13.1)
include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(ruuvi_node)

target_sources(app PRIVATE src/main.c)
zephyr_include_directories(src)

add_subdirectory(src/led_controller)
add_subdirectory(src/uart_controller)
#add_subdirectory(src/gps_controller)
#add_subdirectory(src/modem_controller)
#add_subdirectory(src/watchdog)