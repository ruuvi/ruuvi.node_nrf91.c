# ruuvi.node_nrf91.c
nRF91 project for Ruuvi Node
 
# Cloning and building
mkdir ruuvi_node_sdk && cd ruuvi_node_sdk
west init -m https://github.com/ruuvi/ruuvi.node_nrf91.c
west update
cd ruuvi_node
west build -b nrf9160_pca10090ns

# Licenses
Ruuvi code is BSD-3 licensed. 

The GitHub workflow is adapted from [@bifravst/firmware](https://github.com/bifravst/firmware) and is BSD-5-Clause-Nordic licensed.