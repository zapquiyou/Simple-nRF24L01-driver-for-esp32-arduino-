#include "nRF24L01.h"

bool nrf24flag;
byte nrfbuffer[33] = { 0 };

void setup() {
	Serial.begin(115200);
	nrf24flag = nrf24l01.Init();
	if (nrf24flag)
	{
		Serial.println("Init Succeed");
	}
	else
	{
		Serial.println("Init Failed");
	}
	nrf24l01.RX_Mode();
}

void loop() {
	if (nrf24l01.RxPacket(nrfbuffer) == 0)
	{
		nrfbuffer[32] = '\0';
		Serial.println((char*)nrfbuffer);
		Serial.println("Received!");
	}
}
