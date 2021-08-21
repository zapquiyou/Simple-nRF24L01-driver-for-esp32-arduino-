/*******************************************************************************************************

Simple nRD24L01 driver for esp32(arduino) V1.0
Made by zapquiyou, China

********************************************************************************************************/

#ifndef NRF24L01_H
#define NRF24L01_H

#include <Arduino.h>

#define NRF_READ_REG    0x00  //Read the configuration register, the lower 5 bits are the register address
#define NRF_WRITE_REG   0x20  //Write configuration register, the lower 5 bits are the register address
#define RD_RX_PLOAD     0x61  //Read RX valid data, 1~32 bytes
#define WR_TX_PLOAD     0xA0  //Write RX valid data, 1~32 bytes
#define FLUSH_TX        0xE1  //Clear the TX FIFO register. Used in TX mode
#define FLUSH_RX        0xE2  //Clear the RX FIFO register. Used in RX mode
#define REUSE_TX_PL     0xE3  //The last data is reused, CE is high, and data packets are continuously sent.
#define NOP             0xFF  //Null operation

#define CONFIG          0x00  //Configuration Register
#define EN_AA           0x01  //Enable 'Auto Acknowledgment' Function
#define EN_RXADDR       0x02  //Enabled RX Addresses
#define SETUP_AW        0x03  //Setup of Address Widths.
#define SETUP_RETR      0x04  //Setup of Automatic Retransmission.
#define RF_CH           0x05  //RF Channel.
#define RF_SETUP        0x06  //RF Setup Register.
#define NRF24L01_STATUS 0x07  //Status Register.
#define MAX_TX  		0x10  //Interruption of the maximum number of transmissions is reached.
#define TX_OK   		0x20  //Interruption of send complete.
#define RX_OK   		0x40  //Interruption of receive data.

#define OBSERVE_TX      0x08  //Transmit observe register, bit7:4, Count lost packets; bit3:0, Count retransmitted packets.
#define CD              0x09  //Received Power Detector, bit0, Received Detector;
#define RX_ADDR_P0      0x0A  //Receive address data pipe 0. 5 Bytes maximum length.
#define RX_ADDR_P1      0x0B  //Receive address data pipe 1. 5 Bytes maximum length.
#define RX_ADDR_P2      0x0C  //Receive address data pipe 2. MSBytes are equal to RX_ADDR_P1.
#define RX_ADDR_P3      0x0D  //Receive address data pipe 3. MSBytes are equal to RX_ADDR_P1.
#define RX_ADDR_P4      0x0E  //Receive address data pipe 4. MSBytes are equal to RX_ADDR_P1.
#define RX_ADDR_P5      0x0F  //Receive address data pipe 5. MSBytes are equal to RX_ADDR_P1.
#define TX_ADDR         0x10  //Transmit address. Used for a PTX device only.
#define RX_PW_P0        0x11  //Number of bytes in RX payload in data pipe 0 (1 to 32 bytes), 0 Pipe not used.
#define RX_PW_P1        0x12  //Number of bytes in RX payload in data pipe 1 (1 to 32 bytes), 0 Pipe not used.
#define RX_PW_P2        0x13  //Number of bytes in RX payload in data pipe 2 (1 to 32 bytes), 0 Pipe not used.
#define RX_PW_P3        0x14  //Number of bytes in RX payload in data pipe 3 (1 to 32 bytes), 0 Pipe not used.
#define RX_PW_P4        0x15  //Number of bytes in RX payload in data pipe 4 (1 to 32 bytes), 0 Pipe not used.
#define RX_PW_P5        0x16  //Number of bytes in RX payload in data pipe 5 (1 to 32 bytes), 0 Pipe not used.
#define NRF_FIFO_STATUS 0x17  //FIFO Status Register

#define TX_ADR_WIDTH    5   	//5-byte address width
#define RX_ADR_WIDTH    5   	//5-byte address width
#define TX_PLOAD_WIDTH  32  	//32-byte user data width
#define RX_PLOAD_WIDTH  32  	//32-byte user data width

class NRF24L01
{
public:
    byte SCK_PIN = 14;
    byte MISO_PIN = 12;
    byte MOSI_PIN = 13;
    byte SS_PIN = 15;
    byte CE_PIN = 33;
    byte IRQ_PIN = 34;
    const byte TX_ADDRESS[TX_ADR_WIDTH] = { 0x34,0x43,0x10,0x10,0x01 }; 
    const byte RX_ADDRESS[RX_ADR_WIDTH] = { 0x34,0x43,0x10,0x10,0x01 }; 

    void setPin(byte sck, byte miso, byte mosi, byte ss, byte ce, byte irq);
    void SpiStart();
    void SpiEnd();
    void RegConfigSettings();
    bool Init();
    void RX_Mode();
    void TX_Mode();
    void SB1_Mode();
    unsigned char Write_Buf(unsigned char reg, unsigned char* buf, unsigned char len);
    unsigned char Read_Buf(unsigned char reg, unsigned char* buf, unsigned char len);
    unsigned char Read_Reg(unsigned char reg);
    unsigned char Write_Reg(unsigned char reg, unsigned char value);
    bool Check();
    unsigned char TxPacket(unsigned char* txbuf);
    unsigned char RxPacket(unsigned char* rxbuf);
};

extern NRF24L01 nrf24l01;

#endif 
