/*
 * tinySPI.h
 *
 * Created: 13/4/2020 10:53:53 AM
 *  Author: kyi
 */ 


#ifndef TINYSPI_H_
#define TINYSPI_H_

#include <util/atomic.h>
#include <util/delay.h>

  #define SPI_DDR_PORT DDRA
  #define SPI_PORT PORTA
  #define NSS_PIN PA3
  #define NSS_DD_PIN DDA3
  #define USCK_DD_PIN DDA4
  #define DO_DD_PIN DDA5
  #define DI_DD_PIN DDA6
    
  // SPI data modes
  #define SPI_MODE0 0x00
  #define SPI_MODE1 0x04
  
void tinySPIinit(void);
void setDataMode(uint8_t spiDataMode);
void spiWrite(uint8_t spiAdd, uint8_t spiData);
void transfer(uint8_t spiData);
void spiWait(void);
uint8_t spiRead(uint8_t spiAdd);
void spiBurstWrite(uint8_t reg, const uint8_t* src, uint8_t len);

#endif /* TINYSPI_H_ */