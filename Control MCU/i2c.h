/************************************************************************************
 * Module: I2C
 *
 * Filename: i2c.h
 *
 * Description: Header file for the i2c driver
 *
 * Created on: Oct 31, 2021
 *
 * Author: Mostafa Mahmoud
 ************************************************************************************/

#ifndef I2C_H_
#define I2C_H_
#include "std_types.h"
/***************************************************************************
 *                      Preprocessor Macros
 ***************************************************************************/

/* I2C Status Bits in the TWSR Register */
#define TWI_START         0x08 /* start has been sent */
#define TWI_REP_START     0x10 /* repeated start */
#define TWI_MT_SLA_W_ACK  0x18 /* Master transmit ( slave address + Write request ) to slave + ACK received from slave. */
#define TWI_MT_SLA_R_ACK  0x40 /* Master transmit ( slave address + Read request ) to slave + ACK received from slave. */
#define TWI_MT_DATA_ACK   0x28 /* Master transmit data and ACK has been received from Slave. */
#define TWI_MR_DATA_ACK   0x50 /* Master received data and send ACK to slave. */
#define TWI_MR_DATA_NACK  0x58 /* Master received data but doesn't send ACK to slave. */


/**************************************************************************
 * 							Type Definitions
 **************************************************************************/
typedef struct
{
	uint8 address;
	uint32 bit_rate;

}TWI_CONFIG;

/****************************************************************************
 * 							Function Prototypes
 ****************************************************************************/
void TWI_init(TWI_CONFIG* config);
void TWI_start();
void TWI_stop();
void TWI_writeByte(uint8 data);
uint8 TWI_readByteWithACK();
uint8 TWI_readByteWithNACK();
uint8 TWI_getStatus();


#endif /* I2C_H_ */
