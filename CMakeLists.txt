cmake_minimum_required(VERSION 3.13.1)

# Re-direct the directory where the 'boards' directory is found from
# $ZEPHYR_BASE to this directory.
set(BOARD_ROOT ${CMAKE_CURRENT_LIST_DIR})

# This sample is only compatible with ruuvi_node because it has
# re-directed the 'boards' directory to a directory that only contains
# this board.
set(BOARD nrf9160_pca10090ns)

include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(ruuvi_node)

target_sources(app PRIVATE src/main.c)
zephyr_include_directories(src)

add_subdirectory(src/led_controller)
add_subdirectory(src/uart_controller)
add_subdirectory(src/gps_controller)
add_subdirectory(src/modem_controller)
add_subdirectory(src/http_controller)
add_subdirectory(src/data_parser)
add_subdirectory(src/time_handler)
#add_subdirectory(src/watchdog)