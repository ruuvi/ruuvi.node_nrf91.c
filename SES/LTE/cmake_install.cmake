# Install script for directory: /home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c

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
    set(CMAKE_INSTALL_CONFIG_NAME "LTE")
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
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/LTE/zephyr/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/LTE/src/adv_post/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/LTE/src/api/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/LTE/src/led_controller/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/LTE/src/uart_controller/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/LTE/src/gps_controller/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/LTE/src/http_controller/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/LTE/src/time_handler/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/LTE/src/watchdog/cmake_install.cmake")
  include("/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/LTE/components/ruuvi.endpoints.c/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/ruuviUser/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/LTE/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
