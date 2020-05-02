# ruuvi.node_nrf91.c
nRF91 project for Ruuvi Node

# Setting up the toolchain
Nordic Semiconductor keeps up-to-date instructions on how to setup the toolchain for 
Linux, Mac OSX and Windows. Instruction can be found at: https://developer.nordicsemi.com/nRF_Connect_SDK/doc/1.2.0/nrf/getting_started.html.

Ensure that NCS version 1.2.0 is used as Ruuvi Node is confirmed working with that.

When cloning the repo ensure that you run the following command to clone v1.2.0:

```bash
west init -m https://github.com/NordicPlayground/fw-nrfconnect-nrf --mr v1.2.0
```

# Cloning and building
This assumes that you are running linux and followed the instructions found at the link above and cloned NRF Conect SDK into ncs.

```bash
cd ~/ncs/nrf/applications
mkdir ruuvi_node_sdk && cd ruuvi_node_sdk
git clone https://github.com/${GITHUB_REPOSITORY} ruuvi.node_nrf91.c
cd ruuvi.node_nrf91.c
west build
```

# Licenses
Ruuvi code is BSD-3 licensed. 

The GitHub workflow is adapted from [@bifravst/firmware](https://github.com/bifravst/firmware) and is BSD-5-Clause-Nordic licensed.
