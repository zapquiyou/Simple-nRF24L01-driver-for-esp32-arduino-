# Simple nRD24L01 driver for esp32(arduino) V1.0
Hello everyone, I'm zapquiyou. The driver is coded by myself for some projects. Becasue I can't find a nRD24L01 driver easy to use for esp32(arduino), I made and shared this.

## How to use in code
You can put the lib files in your project directory and include the header file.

## How to connect the hardware
The default connection is below:  
esp32_pin&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; nRD24L01_pin  
 GPIO15  <------------>  CSN  
 GPIO14  <------------>  SCK  
 GPIO12  <------------>  MISO  
 GPIO13  <------------>  MOSI  
 GPIO33  <------------>  CE   
 GPIO34  <------------>  IRQ  
 
You can change the pins' connection by modify the header file or call the setPin method before initialization

Please refer to the example for data sending and receiving programming

## PS
If this lib helps you in the academic papers or business, please mention and remind me. That may be helpful to me and encourage me to share more works, thanks!