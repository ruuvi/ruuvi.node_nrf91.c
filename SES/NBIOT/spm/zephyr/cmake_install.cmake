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
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/spm/zephyr/arch/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/spm/zephyr/lib/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/spm/zephyr/soc/arm/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/spm/zephyr/boards/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/spm/zephyr/subsys/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/spm/zephyr/drivers/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/spm/modules/nrf/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/spm/modules/mcuboot/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/spm/modules/mcumgr/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/spm/modules/nrfxlib/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/spm/modules/cmsis/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/spm/modules/canopennode/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/spm/modules/civetweb/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/spm/modules/fatfs/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/spm/modules/nordic/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/spm/modules/st/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/spm/modules/libmetal/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/spm/modules/lvgl/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/spm/modules/mbedtls/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/spm/modules/open-amp/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/spm/modules/loramac-node/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/spm/modules/openthread/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/spm/modules/segger/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/spm/modules/tinycbor/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/spm/modules/tinycrypt/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/spm/modules/littlefs/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/spm/modules/mipi-sys-t/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/spm/modules/nrf_hw_models/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/spm/zephyr/kernel/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/spm/zephyr/cmake/flash/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/spm/zephyr/cmake/usage/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/NBIOT/spm/zephyr/cmake/reports/cmake_install.cmake")

endif()

