/*
 * bcr_dac.c
 *
 * Created: 4/09/2012 10:01:23 PM
 *  Author: Mitch
 */ 

#include <asf.h>

static struct spi_device spi_device_conf = {
	.id = DAC_SS
};

void bcr_dac_init(void) {
	//Init pins
	ioport_configure_pin(DAC_SS, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
	ioport_configure_pin(DAC_MOSI, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
	ioport_configure_pin(DAC_MISO, IOPORT_DIR_INPUT);
	ioport_configure_pin(DAC_SCK, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
	
	//Init module
	spi_master_init(&SPIC);
	spi_master_setup_device(&SPIC, &spi_device_conf, SPI_MODE_0, DAC_BAUD, 0);
	spi_enable(&SPIC);
}

//Updates a single DAC pin
//V_OUTA -> pin=1
//V_OUTH -> pin=8
static void bcr_dac_update_pin(uint8_t pin, uint16_t value) {
	uint16_t dac_word = ((pin&0xF)<<12) | ((value&0x3FF)<<2);
	uint8_t spi_packet[2] = {(uint8_t)(dac_word>>8), (uint8_t)dac_word};
		
	//Write packet over spi
	spi_select_device(&SPIC, &spi_device_conf);
	spi_write_packet(&SPIC, spi_packet, 2);
	spi_deselect_device(&SPIC, &spi_device_conf);
}

//Updates all 8 DAC pins
void bcr_dac_update(void) {
	for (uint16_t i=0; i<8; i++)
		//bcr_dac_update_pin(i+1, 255);
		bcr_dac_update_pin(i+1, *(((uint16_t*)&bcr_dac)+i));
}