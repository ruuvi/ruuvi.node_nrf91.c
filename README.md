# ruuvi.node_nrf91.c
nRF91 project for Ruuvi Node

# Build Environment
Ruuvi Node is developed using nRF Connect SDK(NCS) V1.4.0.

## NCS
Nordic Semiconductor keeps up-to-date instructions on how to setup the SDK for 
Linux, Mac OSX and Windows. Instructions can be found at https://developer.nordicsemi.com/nRF_Connect_SDK/doc/1.4.0/nrf/getting_started.html

# Cloning
To clone the directory follow the following. the examples given assume you are using linux.
All commands assume that nRF Connect SDK has been setup as per the instructions in the above link.

```bash
cd ~/ncs/nrf/applications
git clone https://github.com/ruuvi/ruuvi.node_nrf91.c.git
cd ruuvi.node_nrf91.c
```

# Building West
Once in the project directory, make can be used to build the different variances of the firmware and flash them to a board.

```bash
# Makes all required build directories and makes all FW variations
make

# Makes the LTE variance, then flashes it
make lte
make flash_lte

# Makes the debug LTE variance, then flashes it
make lte_debug
make flash_lte_debug

# Makes the NB-IoT variance, then flashes it
make nbiot
make flash_nbiot

# Makes the debug LTE variance, then flashes it
make nbiot_debug
make flash_nbiot_debug

# Cleans all build directories
make clean
```

Copies of the build files will be copied to the ruuvi.node_nrf91.c/ruuvi_node_builds/
These files can be used later on with nRF Connect or nrfjprog to flash the board later or on another computer.

# Building Segger

## Installing SES
Segger Embedded Studio Nordic Edition is required to be able to work with NCS projects. Instruction on installing this can be found at: https://developer.nordicsemi.com/nRF_Connect_SDK/doc/1.4.0/nrf/gs_installing.html#installing-ses-nordic-edition

## Preparing the project
Due to the imorting scripts and the project being based on cmake and ninja the files are customised to the user. A script has been that can be run to chaneg the default username 'ruuviUser' to your. NOTE: this assumes that you are working on linux

To run the script run the following:

```bash
cd ~/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/

# Replace bob with your username that is you home directory eg /home/bob
./prepare.sh bob
```

By running the script, all files in the SES project folder will have ruuviUser changed to your own and symlinks will also be edited.

## Opening the project in SES
Ensure that SES Nordic Edition is open. Then go to File > Open Solution and navigate to ~/ncs/nrf/applications/ruuvi.node_nrf91.c/SES/LTE_DEBUG/ruuvi.node_nrf91.c.emProject

NOTE: The LTE_DEBUG.emProject is a solution within the project and should not be opened directly as the build will fail.

# Licenses
Ruuvi code is BSD-3 licensed. 

The GitHub workflow is adapted from [@bifravst/firmware](https://github.com/bifravst/firmware) and is BSD-5-Clause-Nordic licensed.
