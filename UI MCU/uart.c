/************************************************************************************
 * Module: UART
 *
 * Filename: uart.c
 *
 * Description: source file for the UART driver
 *
 * Created on: Oct 16, 2021
 *
 * Author: Mostafa Mahmoud
 ************************************************************************************/

#include "uart.h"
#include <avr/io.h>
#include "common_macros.h"
/*
 * Description:
 * Function to initialize the UART device:
 * 1-Setting up the frame format like no. of data bits , parity bit type and no. of stop bits
 * 2-Enable the UART
 * 3-Setup the UART baud rate
 */
void UART_init(Uart_ConfigType * config)
{
	uint16 ubrr_value = 0;
	/*U2X=1 for double speed*/
	UCSRA=(1<<U2X);
	/*
	 * Configuring the UCSRB register:
	 * RXCIE=0 to disable receive interrupt
	 * TXCIE=0 to disable transmit interrupt
	 * RXEN=1 receive enable
	 * TXEN=1 transmit enable
	 * UDRIE=0 to Disable USART Data Register Empty Interrupt Enable
	 * RXB8 not used for 8-bit data mode
	 * TXB8 not used for 8-bit data mode
	 */
	UCSRB=(1<<RXEN)|(1<<TXEN);
	/*Setup UCSZ2 for the number of bits*/
	UCSRB=(UCSRB & 0xFB)|(config->bits & 0x04);

	/*
	 * Configuring the UCSRC register:
	 * URSEL=1 to enable writing in the UCSRC register
	 * UMSEL=0 for asynchronous mode
	 * UCPOL not used for asynchronous mode
	 */
	UCSRC=(1<<URSEL);
	/*Setup the parity bits : UPM1 & UPM0 bits*/
	UCSRC=(UCSRC & 0xCF)|((config->parity & 0x03)<<4);
	/*Setup the number of stop-bits : USBS bit*/
	UCSRC=(UCSRC & 0xF7)|((config->stop & 0x01)<<3);
	/*Setup the number of bits : UCSZ1 & UCSZ0 bits*/
	UCSRC=(UCSRC & 0xF9)|((config->bits & 0x03)<<1);

	/*Calculating the UBRR register value*/
	ubrr_value = (uint16)(((F_CPU / (config->BaudRate * 8UL))) - 1);

	UBRRH=ubrr_value>>8;
	UBRRL=ubrr_value;
}
/*
 * Description:
 * Function to send a byte to another UART device
 */
void UART_sendByte(uint8 data)
{
	UDR=data;
	while(BIT_IS_CLEAR(UCSRA,TXC));
	SET_BIT(UCSRA,TXC);
}
/*
 * Description:
 * Function to receive a byte from another UART device
 */
uint8 UART_receiveByte(void)
{
	while(BIT_IS_CLEAR(UCSRA,RXC));
	return UDR;
}
/*
 *Description:
 *Function to send a string to another UART device
 */
void UART_sendString(uint8 *str)
{
	uint8 i=0;
	while(str[i]!='\0')
	{
		UART_sendByte(str[i]);
		i++;
	}
}
/*
 * Description:
 * Function to receive a string from another UART device until the '#' symbol
 */
void UART_receiveString(uint8 *str)
{
	uint8 i=0;
	/*Receive the first byte*/
	str[i]=UART_receiveByte();
	while(str[i]!='#')
	{
		i++;
		str[i]=UART_receiveByte();
	}
	str[i]='\0';
}
