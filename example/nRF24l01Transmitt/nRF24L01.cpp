/*******************************************************************************************************

Simple nRD24L01 driver for esp32(arduino) V1.0
Made by zapquiyou, China

********************************************************************************************************/

#include <SPI.h>
#include "nRF24L01.h"
#include <Arduino.h>

SPIClass SPI2(HSPI);

void NRF24L01::setPin(byte sck, byte miso, byte mosi, byte ss, byte ce, byte irq)
{
	SCK_PIN = sck;
	MISO_PIN = miso;
	MOSI_PIN = mosi;
	SS_PIN = ss;
	CE_PIN = ce;
	IRQ_PIN = irq;
}

void NRF24L01::SpiStart()
{
	pinMode(SCK_PIN, OUTPUT);
	pinMode(MOSI_PIN, OUTPUT);
	pinMode(MISO_PIN, INPUT);
	pinMode(SS_PIN, OUTPUT);
	pinMode(CE_PIN, OUTPUT);
	pinMode(IRQ_PIN, INPUT);

	SPI2.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);
}

void NRF24L01::SpiEnd()
{
	SPI2.endTransaction();
	SPI2.end();
	digitalWrite(SCK_PIN, LOW);
}

void NRF24L01::RegConfigSettings()
{
	digitalWrite(CE_PIN, LOW);
	Write_Reg(NRF_WRITE_REG + CONFIG, 0x0A);
	ets_delay_us(5000);
	Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, (unsigned char*)RX_ADDRESS, RX_ADR_WIDTH);
	Write_Buf(NRF_WRITE_REG + TX_ADDR, (unsigned char*)TX_ADDRESS, TX_ADR_WIDTH);
	Write_Reg(NRF_WRITE_REG + EN_AA, 0x01);
	Write_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01);
	Write_Reg(NRF_WRITE_REG + RF_CH, 40);
	Write_Reg(NRF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH);
	Write_Reg(NRF_WRITE_REG + RF_SETUP, 0x0f);
	digitalWrite(CE_PIN, HIGH);
	ets_delay_us(2000);
}

bool NRF24L01::Init()
{
	bool flag;
	SpiStart();
	digitalWrite(SS_PIN, HIGH);
	digitalWrite(SCK_PIN, HIGH);
	digitalWrite(MOSI_PIN, LOW);
	SpiEnd();
	flag = Check();
	RegConfigSettings();
	return flag;
}

void NRF24L01::RX_Mode()
{
	digitalWrite(CE_PIN, LOW);
	Write_Reg(NRF_WRITE_REG + CONFIG, 0x0f);
	digitalWrite(CE_PIN, HIGH);
	ets_delay_us(200);
}

void NRF24L01::TX_Mode()
{
	digitalWrite(CE_PIN, LOW);
	Write_Reg(NRF_WRITE_REG + CONFIG, 0x0e);
	digitalWrite(CE_PIN, HIGH);
	ets_delay_us(200);
}

void NRF24L01::SB1_Mode()
{
	digitalWrite(CE_PIN, LOW);
}

unsigned char NRF24L01::Write_Buf(unsigned char reg, unsigned char* buf, unsigned char len)
{
	unsigned char status;
	SpiStart();
	digitalWrite(SS_PIN, LOW);
	ets_delay_us(3);
	status = SPI2.transfer(reg);
	for (unsigned char i = 0; i < len; i++)
	{
		SPI2.transfer(*buf++);
	}
	ets_delay_us(3);
	digitalWrite(SS_PIN, HIGH);
	SpiEnd();
	return status;
}

unsigned char NRF24L01::Read_Buf(unsigned char reg, unsigned char* buf, unsigned char len)
{
	unsigned char status;
	SpiStart();
	digitalWrite(SS_PIN, LOW);
	ets_delay_us(3);
	status = SPI2.transfer(reg);
	for (unsigned char i = 0; i < len; i++)
	{
		buf[i] = SPI2.transfer(0);
	}
	ets_delay_us(3);
	digitalWrite(SS_PIN, HIGH);
	SpiEnd();
	return 0;
}

unsigned char NRF24L01::Read_Reg(unsigned char reg)
{
	unsigned char value;
	SpiStart();
	digitalWrite(SS_PIN, LOW);
	ets_delay_us(3);
	SPI2.transfer(reg);
	value = SPI2.transfer(0);
	ets_delay_us(3);
	digitalWrite(SS_PIN, HIGH);
	SpiEnd();
	return value;
}

unsigned char NRF24L01::Write_Reg(unsigned char reg, unsigned char value)
{
	unsigned char status;
	SpiStart();
	digitalWrite(SS_PIN, LOW);
	ets_delay_us(3);
	status = SPI2.transfer(reg);
	SPI2.transfer(value);
	ets_delay_us(3);
	digitalWrite(SS_PIN, HIGH);
	SpiEnd();
	return status;
}

bool NRF24L01::Check()
{
	unsigned char buf[5] = { 0xa5,0xa5,0xa5,0xa5,0xa5 };
	bool flag = true;
	Write_Buf(NRF_WRITE_REG + TX_ADDR, buf, 5);
	Read_Buf(NRF_READ_REG + TX_ADDR, buf, 5);
	for (unsigned char i = 0; i < 5; i++)
	{
		if (buf[i] != 0xa5)
		{
			flag = false;
			break;
		}
	}
	return flag;
}

unsigned char NRF24L01::TxPacket(unsigned char* txbuf)
{
	unsigned char status;
	digitalWrite(CE_PIN, LOW);
	Write_Buf(WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH);
	digitalWrite(CE_PIN, HIGH);
	ets_delay_us(20);
	digitalWrite(CE_PIN, LOW);
	Serial.println("Sending");
	while (digitalRead(IRQ_PIN) != LOW);
	status = Read_Reg(NRF24L01_STATUS);
	Write_Reg(NRF_WRITE_REG + NRF24L01_STATUS, status);
	if (status&MAX_TX)
	{
		Write_Reg(FLUSH_TX, 0xff);
		return MAX_TX;
	}
	if (status&TX_OK)
	{
		return TX_OK;
	}
	return 0xff;
}

unsigned char NRF24L01::RxPacket(unsigned char* rxbuf)
{
	unsigned char status;
	status = Read_Reg(NRF24L01_STATUS);
	Write_Reg(NRF_WRITE_REG + NRF24L01_STATUS, status);
	if (status & RX_OK)
	{
		Read_Buf(RD_RX_PLOAD, rxbuf, RX_PLOAD_WIDTH);
		Write_Reg(FLUSH_RX, 0xff);
		return 0;
	}
	return 1;
}

NRF24L01 nrf24l01;