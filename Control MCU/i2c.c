/************************************************************************************
 * Module: I2C
 *
 * Filename: i2c.c
 *
 * Description: Source file for the i2c driver
 *
 * Created on: Oct 31, 2021
 *
 * Author: Mostafa Mahmoud
 ************************************************************************************/
#include <avr/io.h>
#include "common_macros.h"
#include "i2c.h"
/****************************************************************************
 * 							Function Definitions
 ****************************************************************************/
void TWI_init(TWI_CONFIG* config)
{

	TWBR = (((F_CPU)/(config->bit_rate))-16)*(0.5);
	TWSR = 0x00;
	TWAR = config->address<<1;
	TWCR=(1<<TWEN);
}

void TWI_start()
{
	TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while(BIT_IS_CLEAR(TWCR,TWINT));
}

void TWI_stop()
{
	TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

void TWI_writeByte(uint8 data)
{
	TWDR=data;
	TWCR=(1<<TWINT)|(1<<TWEN);
	while(BIT_IS_CLEAR(TWCR,TWINT));
}

uint8 TWI_readByteWithACK()
{
	TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while(BIT_IS_CLEAR(TWCR,TWINT));
	return TWDR;
}

uint8 TWI_readByteWithNACK()
{
	TWCR=(1<<TWINT)|(1<<TWEN);
	while(BIT_IS_CLEAR(TWCR,TWINT));
	return TWDR;
}

uint8 TWI_getStatus()
{
	uint8 status=0;
	status=TWSR & 0xF8;
	return status;
}
