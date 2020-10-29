# Install script for directory: /home/ruuviUser/ncs/zephyr

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "NBIOT")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/zephyr/arch/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/zephyr/lib/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/zephyr/soc/arm/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/zephyr/boards/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/zephyr/subsys/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/zephyr/drivers/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/modules/nrf/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/modules/mcuboot/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/modules/mcumgr/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/modules/nrfxlib/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/modules/cmsis/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/modules/canopennode/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/modules/civetweb/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/modules/fatfs/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/modules/nordic/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/modules/st/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/modules/libmetal/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/modules/lvgl/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/modules/mbedtls/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/modules/open-amp/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/modules/loramac-node/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/modules/openthread/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/modules/segger/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/modules/tinycbor/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/modules/tinycrypt/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/modules/littlefs/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/modules/mipi-sys-t/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/modules/nrf_hw_models/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/zephyr/kernel/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/zephyr/cmake/flash/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/zephyr/cmake/usage/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/zephyr/cmake/reports/cmake_install.cmake")

endif()

