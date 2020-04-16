/*
 * tinySPI.c
 *
 * Created: 13/4/2020 10:54:06 AM
 *  Author: kyi
 */ 

#include "tinySPI.h"
#define NSSlow SPI_PORT &= ~(1 << NSS_PIN)
#define NSShi SPI_PORT |= (1 << NSS_PIN);

void tinySPIinit(void)
{
	USICR &= ~(_BV(USISIE) | _BV(USIOIE) | _BV(USIWM1));
	USICR |= _BV(USIWM0)  //3 wire mode
	       | _BV(USICS1)  //
		   | _BV(USICLK); // external, positive edge SPI mode 0
		   
	SPI_DDR_PORT |= _BV(USCK_DD_PIN);   // set the USCK pin as output
	SPI_DDR_PORT |= _BV(DO_DD_PIN);     // set the DO pin as output
	SPI_DDR_PORT &= ~_BV(DI_DD_PIN);    // set the DI pin as input
	SPI_DDR_PORT |= _BV(NSS_DD_PIN);    // set the NSS pin as output
	SPI_PORT |= (1 << NSS_PIN);          // NSS high
}


void setDataMode(uint8_t spiDataMode)
{
	if (spiDataMode == SPI_MODE1)
	USICR |= _BV(USICS0);
	else
	USICR &= ~_BV(USICS0);
}

void spiWrite(uint8_t spiAdd, uint8_t spiData)
{
	// set SS Low
	// transfer add
	NSSlow;
	transfer(0x80|spiAdd);  // *kzw - write mode
	transfer(spiData);
	
	NSShi;
	//return status
}

void transfer(uint8_t spiData)
{
	USIDR = spiData;	
	spiWait();
//	return USIDR;
}
uint8_t spiRead(uint8_t spiAdd)
{
	USIDR = spiAdd;
	NSSlow;
	spiWait();
		
	spiWait();
	NSShi;
	return USIDR;
}
void spiWait()
{
	USISR = _BV(USIOIF);                // clear counter and counter overflow interrupt flag
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)   // ensure a consistent clock period
	{
//		while ( !(USISR & _BV(USIOIF)) ) USICR |= _BV(USITC);
		while ( !(USISR & _BV(USIOIF)) ){USICR |= _BV(USITC);}
	}
	
}
void spiBurstWrite(uint8_t spiAdd, const uint8_t* src, uint8_t len)
{
	uint8_t status = 0;
	//ATOMIC_BLOCK(ATOMIC_RESTORESTATE)   // ensure a consistent clock period
	NSSlow;
	transfer(0x80| spiAdd);
	
	while (len--)
    {
		transfer(*src++);
	}
	NSShi;
//	return status;
}

void end()
{
	USICR &= ~(_BV(USIWM1) | _BV(USIWM0));
}
