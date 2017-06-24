/* ENC28J60 hardware implementation for ESP devices.
*/
#include <Arduino.h>
#include "enchw.h"
#include <SPI.h>

#define SS_PIN 5

static volatile uint8_t j=0;
#define pause() while(++j)

void enchw_setup(enchw_device_t *dev) {
	SPI.begin();
	SPI.setBitOrder(SPI_MSBFIRST);
    pinMode(SS_PIN, OUTPUT);
	printf("enchw_setup\n");
}

void enchw_select(enchw_device_t *dev) {
	printf("chip enable\n");
    digitalWrite(SS_PIN, LOW);
}

void enchw_unselect(enchw_device_t *dev) {
	printf("chip disable\n");
    digitalWrite(SS_PIN, HIGH);
}

uint8_t enchw_exchangebyte(enchw_device_t *dev, uint8_t byte) {
	SPI.transfer(byte);
	return SPI.transfer(0x00);
}

