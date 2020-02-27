# ruuvi.node_nrf91.c
nRF91 project for Ruuvi Node

# Setting up the toolchain
Nordic Semiconductor keeps up-to-date instructions on how to setup the toolchain for 
Linux, Mac OSX and Windows. Download [nRF Connect for Desktop](https://www.nordicsemi.com/Software-and-tools/Development-Tools/nRF-Connect-for-desktop) and follow getting started -plugin instructions. 

# Cloning and building
mkdir ruuvi_node_sdk && cd ruuvi_node_sdk
west init -m https://github.com/ruuvi/ruuvi.node_nrf91.c
west update
cd ruuvi_node
west build

# Licenses
Ruuvi code is BSD-3 licensed. 

The GitHub workflow is adapted from [@bifravst/firmware](https://github.com/bifravst/firmware) and is BSD-5-Clause-Nordic licensed.