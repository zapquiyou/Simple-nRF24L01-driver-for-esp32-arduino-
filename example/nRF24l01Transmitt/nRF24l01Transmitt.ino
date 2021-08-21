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
}

void loop() {
	if (Serial.available())
	{
		int len = Serial.readBytesUntil('\n', nrfbuffer, TX_PLOAD_WIDTH);
		nrfbuffer[len] = '\0';
		nrf24l01.TX_Mode();
		if (nrf24l01.TxPacket(nrfbuffer) == TX_OK)
		{
			Serial.println("Send Succeed");
		}
		else
		{
			Serial.println("Send Failed");
		}
	}
}
