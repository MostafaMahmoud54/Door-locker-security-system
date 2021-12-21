/************************************************************************************
 * Module: LCD
 *
 * Filename: lcd.c
 *
 * Description: source file for the LCD driver
 *
 * Created on: Oct 4, 2021
 *
 * Author: Mostafa Mahmoud
 ************************************************************************************/
#include "gpio.h"
#include "lcd.h"
#include "common_macros.h"
#include <util/delay.h>
/*************************************************************************************
 * 								Function Definitions
 ************************************************************************************/
/*
 * Description:
 * configure the LCD pins using the GPIO
 * setup the LCD data mode
 */
void LCD_init()
{
	GPIO_setupPinDirection(LCD_RS_PORT_ID,LCD_RS_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_RW_PORT_ID,LCD_RW_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_E_PORT_ID,LCD_E_PIN_ID, PIN_OUTPUT);

	GPIO_setupPortDirection(LCD_DATA_PORT_ID, PORT_OUTPUT);

	LCD_sendCommand(LCD_TWO_LINES_EIGHT_BITS_MODE); /* use 2-line lcd + 8-bit Data Mode + 5*7 dot display Mode */
	LCD_sendCommand(LCD_CURSOR_OFF);
	LCD_sendCommand(LCD_CLEAR_COMMAND);
}
/*
 * Description:
 * send a command to the LCD
 */

void LCD_sendCommand(uint8 command)
{
	GPIO_writePin(LCD_RS_PORT_ID,LCD_RS_PIN_ID, LOGIC_LOW); /*Instruction mode RS=0*/
	GPIO_writePin(LCD_RW_PORT_ID,LCD_RW_PIN_ID, LOGIC_LOW); /*Write data RW=0*/
	_delay_ms(1); /*delay for processing Tas=50 nsec*/
	GPIO_writePin(LCD_E_PORT_ID,LCD_E_PIN_ID, LOGIC_HIGH); /*Enable LCD*/
	_delay_ms(1); /*delay for processing Tpw-Tdsm=190 nsec*/
	GPIO_writePort(LCD_DATA_PORT_ID,command); /*Send command on the DS*/
	_delay_ms(1); /*delay for processing Tdsw=100 nsec*/
	GPIO_writePin(LCD_E_PORT_ID,LCD_E_PIN_ID, LOGIC_LOW); /*Disable the LCD*/
	_delay_ms(1); /*delay for processing Th=13 nsec*/
}
/*
 * Description:
 * Display a char on the screen
 */

void LCD_displayCharacter(uint8 data)
{
	GPIO_writePin(LCD_RS_PORT_ID,LCD_RS_PIN_ID, LOGIC_HIGH); /*Data mode RS=1*/
	GPIO_writePin(LCD_RW_PORT_ID,LCD_RW_PIN_ID, LOGIC_LOW); /*Write data RW=0*/
	_delay_ms(1); /*delay for processing Tas=50 nsec*/
	GPIO_writePin(LCD_E_PORT_ID,LCD_E_PIN_ID, LOGIC_HIGH); /*Enable LCD*/
	_delay_ms(1); /*delay for processing Tpw-Tdsm=190 nsec*/
	GPIO_writePort(LCD_DATA_PORT_ID,data); /*Send data on the DS*/
	_delay_ms(1); /*delay for processing Tdsw=100 nsec*/
	GPIO_writePin(LCD_E_PORT_ID,LCD_E_PIN_ID, LOGIC_LOW); /*Disable the LCD*/
	_delay_ms(1); /*delay for processing Th=13 nsec*/
}
/*
 * Description:
 * Display a string on the screen
 */
void LCD_displayString(uint8 *str)
{
	uint8 i=0;
	while(str[i]!='\0')
	{
		LCD_displayCharacter(str[i]);
		i++;
	}
}
/*
 * Description:
 * move the cursor to a certain location(row and column)
 */
void LCD_moveCursor(uint8 row,uint8 col)
{
	uint8 LCD_MEMORY_ADDRESS;
	/*Calculate the address*/
	switch(row)
	{
	case 0:
		LCD_MEMORY_ADDRESS=col;
		break;
	case 1:
		LCD_MEMORY_ADDRESS=col+0x40;
		break;
	case 2:
		LCD_MEMORY_ADDRESS=col+0x10;
		break;
	case 3:
		LCD_MEMORY_ADDRESS=col+0x50;
		break;
	}
	LCD_sendCommand(LCD_MEMORY_ADDRESS | LCD_SET_CURSOR_LOCATION);
}
/*
 * Description:
 * Display a string in a certain location (row and column)
 */
void LCD_displayStringRowColumn(uint8 row,uint8 col,const uint8* str)
{
	LCD_moveCursor(row,col); /*move the cursor to a certain location*/
	LCD_displayString(str); /*display the string*/
}
/*
 * Description:
 * clear the screen
 */
void LCD_clearScreen()
{
	LCD_sendCommand(LCD_CLEAR_COMMAND);
}
/*
 * Description:
 * Display a decimal number on the screen
 */
void LCD_intgerToString(int data)
{
	char buff[16]; /*string to save the ascii values*/
	itoa(data,buff,10);
	LCD_displayString(buff); /*display the string*/
}
