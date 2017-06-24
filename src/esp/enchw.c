/* ENC28J60 hardware implementation for ESP devices.
*/
#include <Arduino.h>
#include "enchw.h"
#include <SPI.h>

#define SS_PIN 5

static volatile uint8_t j=0;
#define pause() while(++j)

void enchw_setup(enchw_device_t __attribute__((unused)) *dev) {
	SPI.begin();
	SPI.setBitOrder(SPI_MSBFIRST);
    pinMode(SS_PIN, OUTPUT);


void enchw_select(enchw_device_t __attribute__((unused)) *dev) {
    digitalWrite(SS_PIN, LOW);
}

void enchw_unselect(enchw_device_t __attribute__((unused)) *dev) {
    digitalWrite(SS_PIN, HIGH);
}

uint8_t enchw_exchangebyte(enchw_device_t __attribute__((unused)) *dev, uint8_t byte) {
	SPI.transfer(byte);
	return result = SPI.transfer(0x00);
}

