# Main makefile of ruuvi.node_nrf91.c

BUILD_DIRECTORY := ruuvi_node_builds

.PHONY: all bootstrap lte lte_debug nbiot nbiot_debug

all: bootstrap lte lte_debug nbiot nbiot_debug

bootstrap:
ifneq ($(BUILD_DIRECTORY)),)
else
	mkdir $(BUILD_DIRECTORY)
endif

lte: bootstrap
ifneq ($(BUILD_DIRECTORY)/lte),)
else
	mkdir $(BUILD_DIRECTORY)/lte)
endif
	@echo Building LTE
	west build -d $(BUILD_DIRECTORY)/lte -- -DCMAKE_BUILD_TYPE=LTE
	cp -v $(BUILD_DIRECTORY)/lte/zephyr/merged.hex $(BUILD_DIRECTORY)/ruuvi-node-nrf9160_pca10090ns-ltem.hex

flash_lte: bootstrap
ifneq ($(BUILD_DIRECTORY)/lte),)
else
	mkdir $(BUILD_DIRECTORY)/lte)
endif
	@echo Building LTE
	west build -d $(BUILD_DIRECTORY)/lte -- -DCMAKE_BUILD_TYPE=LTE
	west flash -d $(BUILD_DIRECTORY)/lte
	date +"Ruuvi Node flashed at: %FT%T%z"

lte_debug: bootstrap
ifneq ($(BUILD_DIRECTORY)/lte_debug),)
else
	mkdir $(BUILD_DIRECTORY)/lte_debug)
endif
	@echo Building LTE DEBUG
	west build -d $(BUILD_DIRECTORY)/lte_debug -- -DCMAKE_BUILD_TYPE=LTE_DEBUG
	cp -v $(BUILD_DIRECTORY)/lte_debug/zephyr/merged.hex $(BUILD_DIRECTORY)/ruuvi-node-nrf9160_pca10090ns-ltem_debug.hex

flash_lte_debug: bootstrap
ifneq ($(BUILD_DIRECTORY)/lte_debug),)
else
	mkdir $(BUILD_DIRECTORY)/lte_debug)
endif
	@echo Building LTE
	west build -d $(BUILD_DIRECTORY)/lte_debug -- -DCMAKE_BUILD_TYPE=LTE_DEBUG
	west flash -d $(BUILD_DIRECTORY)/lte_debug
	date +"Ruuvi Node flashed at: %FT%T%z"

nbiot: bootstrap
ifneq ($(BUILD_DIRECTORY)/nb-iot),)
else
	mkdir $(BUILD_DIRECTORY)/nb-iot)
endif
	@echo Building NB-IoT
	west build -d $(BUILD_DIRECTORY)/nb-iot -- -DCMAKE_BUILD_TYPE=NBIOT
	cp -v $(BUILD_DIRECTORY)/nb-iot/zephyr/merged.hex $(BUILD_DIRECTORY)/ruuvi-node-nrf9160_pca10090ns-nbiot.hex

flash_nbiot: bootstrap
ifneq ($(BUILD_DIRECTORY)/nb-iot),)
else
	mkdir $(BUILD_DIRECTORY)/nb-iot)
endif
	@echo Flashing NB-IoT
	west build -d $(BUILD_DIRECTORY)/nb-iot -- -DCMAKE_BUILD_TYPE=NBIOT
	west flash -d $(BUILD_DIRECTORY)/nb-iot
	date +"Ruuvi Node flashed at: %FT%T%z"

nbiot_debug: bootstrap
ifneq ($(BUILD_DIRECTORY)/nb-iot_debug),)
else
	mkdir $(BUILD_DIRECTORY)/nb-iot_debug)
endif
	@echo Building NB-Iot DEBUG
	west build -d $(BUILD_DIRECTORY)/nb-iot_debug -- -DCMAKE_BUILD_TYPE=NBIOT_DEBUG
	cp -v $(BUILD_DIRECTORY)/nb-iot_debug/zephyr/merged.hex $(BUILD_DIRECTORY)/ruuvi-node-nrf9160_pca10090ns-nbiot_debug.hex

flash_nbiot_debug: bootstrap
ifneq ($(BUILD_DIRECTORY)/nb-iot_debug),)
else
	mkdir $(BUILD_DIRECTORY)/nb-iot_debug)
endif
	@echo Flashing NB-IoT Debug
	west build -d $(BUILD_DIRECTORY)/nb-iot_debug -- -DCMAKE_BUILD_TYPE=NBIOT_DEBUG
	west flash -d $(BUILD_DIRECTORY)/nb-iot_debug
	date +"Ruuvi Node flashed at: %FT%T%z"

clean:
	rm -d -r $(BUILD_DIRECTORY)
