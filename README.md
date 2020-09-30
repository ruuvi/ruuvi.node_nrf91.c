# ruuvi.node_nrf91.c
nRF91 project for Ruuvi Node

# Setting up the toolchain
Nordic Semiconductor keeps up-to-date instructions on how to setup the toolchain for 
Linux, Mac OSX and Windows. Download [nRF Connect for Desktop](https://www.nordicsemi.com/Software-and-tools/Development-Tools/nRF-Connect-for-desktop) and follow getting started -plugin instructions. 

# Cloning and building
```bash
mkdir ncs && cd ncs
west init -m https://github.com/NordicPlayground/fw-nrfconnect-nrf --mr v1.2.0
west update
sudo env "PATH=$PATH" pip3 install -r zephyr/scripts/requirements.txt
sudo env "PATH=$PATH" pip3 install pc_ble_driver_py
sudo env "PATH=$PATH" pip3 install -r nrf/scripts/requirements.txt
sudo apt-get -y remove python-cryptography python3-cryptography
sudo env "PATH=$PATH" pip3 install -r bootloader/mcuboot/scripts/requirements.txt
cd nrf && cd applications
git clone https://github.com/${GITHUB_REPOSITORY} ruuvi.node_nrf91.c
cd ruuvi.node_nrf91.c
west build
```

# Licenses
Ruuvi code is BSD-3 licensed. 

The GitHub workflow is adapted from [@bifravst/firmware](https://github.com/bifravst/firmware) and is BSD-5-Clause-Nordic licensed.
