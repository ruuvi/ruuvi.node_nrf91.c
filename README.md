# ruuvi.node_nrf91.c
nRF91 project for Ruuvi Node

# Build Environment
Ruuvi Node is developed using nRF Connect SDK V1.3.1.

Nordic Semiconductor keeps up-to-date instructions on how to setup the toolchain for 
Linux, Mac OSX and Windows. Instructions can be found at https://developer.nordicsemi.com/nRF_Connect_SDK/doc/1.3.1/nrf/doc_build.html 

# Cloning and building
All commands assume that nRF Connect SDK has been setup as per the instructions in the above link.

```bash
cd ~/ncs/nrf/applications
git clone https://github.com/ruuvi/ruuvi.node_nrf91.c.git
cd ruuvi.node_nrf91.c
```

Once in the project directory, make can be used to build the different variances of the firmware and flash them to a board.

```bash
# Makes all required build directories and makes all FW variations
make

# Makes the LTE variance, then flashes it
make lte
make flash_lte

# Makes the debug LTE variance, , then flashes it
make lte_debug
make flash_lte_debug

# Makes the NB-IoT variance, then flashes it
make nbiot
make flash_nbiot

# Makes the debug LTE variance, , then flashes it
make nbiot_debug
make flash_nbiot_debug

# Cleans all build directories
make clean
```

Also copies of the build files will be copied to the ruuvi.node_nrf91.c/ruuvi_node_builds/
These files can be used later on with nRF Connect or nrfjprog to flash the board later or on another computer.

# Licenses
Ruuvi code is BSD-3 licensed. 

The GitHub workflow is adapted from [@bifravst/firmware](https://github.com/bifravst/firmware) and is BSD-5-Clause-Nordic licensed.
