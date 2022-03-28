/*
 * uart.h
 *
 *  Created on: Oct 23, 2021
 *      Author: Mostafa Mahmoud
 */

#ifndef UART_H_
#define UART_H_

#include "std_types.h"
/*********************************************************************
 * 							Type Definitions
 *********************************************************************/
typedef enum
{
	ONE_BIT,TWO_BITS
}Uart_StopBits;

typedef enum
{
	DISABLED,EVEN_PARITY=2,ODD_PARITY
}Uart_ParityMode;

typedef enum
{
	FIVE_BIT,SIX_BIT,SEVEN_BIT,EIGHT_BIT,NINE_BIT=7
}Uart_NumberOfBits;

typedef struct
{
	Uart_NumberOfBits bits;
	Uart_ParityMode parity;
	Uart_StopBits stop;
	uint32 BaudRate;

}Uart_ConfigType;
/**********************************************************************
 * 						Function Prototypes
 **********************************************************************/
/*
 * Description:
 * Function to initialize the UART device by
 * 1-Setting up the frame format like no. of data bits , parity bit type and no. of stop bits
 * 2-Enable the UART
 * 3-Setup the UART baud rate
 */
void UART_init(Uart_ConfigType * config);
/*
 * Description:
 * Function to send a byte to another UART device
 */
void UART_sendByte(uint8 data);
/*
 * Description:
 * Function to receive a byte from another UART device
 */
uint8 UART_receiveByte(void);
/*
 *Description:
 *Function to send a string to another UART device
 */
void UART_sendString(uint8 *str);
/*
 * Description:
 * Function to receive a string from another UART device until the '#' symbol
 */
void UART_receiveString(uint8 *str);


#endif /* UART_H_ */
