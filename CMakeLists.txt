cmake_minimum_required(VERSION 3.8.2)

if (CMAKE_BUILD_TYPE EQUAL LTE)
  set(CONF_FILE "${CMAKE_CURRENT_SOURCE_DIR}/CONFIGS/LTE.conf")
elseif (CMAKE_BUILD_TYPE EQUAL LTE_DEBUG)
  set(CONF_FILE "${CMAKE_CURRENT_SOURCE_DIR}/CONFIGS/LTE_DEBUG.conf")
elseif (CMAKE_BUILD_TYPE EQUAL NBIOT)
  set(CONF_FILE "${CMAKE_CURRENT_SOURCE_DIR}/CONFIGS/NBIOT.conf")
elseif (CMAKE_BUILD_TYPE EQUAL NBIOT_DEBUG)
  set(CONF_FILE "${CMAKE_CURRENT_SOURCE_DIR}/CONFIGS/NBIOT_DEBUG.conf")
else (NOT CMAKE_BUILD_TYPE)
  set(CONF_FILE "${CMAKE_CURRENT_SOURCE_DIR}/CONFIGS/LTE.conf")
endif()


# This sample is only compatible with ruuvi_node because it has
# re-directed the 'boards' directory to a directory that only contains
# this board.
set(BOARD_ROOT ${CMAKE_CURRENT_LIST_DIR})
# Will be replaced with Ruuvi Node
set(BOARD nrf9160_pca10090ns)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(ruuvi.node_nrf911.c)
zephyr_compile_definitions(PROJECT_NAME=${PROJECT_NAME})
zephyr_compile_definitions(_POSIX_C_SOURCE=200809L)

target_sources(app PRIVATE src/main.c)
zephyr_include_directories(src)

# Include application events and configuration headers
zephyr_library_include_directories(
  src/adv_post
  src/api
  src/led_controller
  src/uart_controller
  src/http_controller
  src/watchdog
  src/ruuvi.endpoints.c
  src/time_handler
  src/gps_controller
  )

add_subdirectory(src/adv_post)
add_subdirectory(src/api)
add_subdirectory(src/led_controller)
add_subdirectory(src/uart_controller)
add_subdirectory(src/gps_controller)
add_subdirectory(src/http_controller)   
add_subdirectory(src/time_handler)
add_subdirectory(src/watchdog)
add_subdirectory(src/ruuvi.endpoints.c)
