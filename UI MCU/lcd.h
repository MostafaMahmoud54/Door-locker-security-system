/************************************************************************************
 * Module: LCD
 *
 * Filename: lcd.h
 *
 * Description: Header file for the LCD driver
 *
 * Created on: Oct 4, 2021
 *
 * Author: Mostafa Mahmoud
 ************************************************************************************/

#ifndef LCD_H_
#define LCD_H_

#include "common_macros.h"
#include "std_types.h"

/*************************************************************************************
 * 									Definitions
 ************************************************************************************/

#define LCD_DATA_PORT_ID PORTC_ID

#define LCD_RS_PORT_ID PORTD_ID
#define LCD_RS_PIN_ID PIN2_ID

#define LCD_RW_PIN_ID PIN3_ID
#define LCD_RW_PORT_ID PORTD_ID

#define LCD_E_PORT_ID PORTD_ID
#define LCD_E_PIN_ID PIN4_ID

#define LCD_CLEAR_COMMAND              0x01
#define LCD_GO_TO_HOME                 0x02
#define LCD_TWO_LINES_EIGHT_BITS_MODE  0x38
#define LCD_TWO_LINES_FOUR_BITS_MODE   0x28
#define LCD_CURSOR_OFF                 0x0C
#define LCD_CURSOR_ON                  0x0E
#define LCD_SET_CURSOR_LOCATION        0x80

/*************************************************************************************
 * 								Function Prototypes
 ************************************************************************************/

/*
 * Description:
 * configure the LCD pins using the GPIO
 * setup the LCD data mode
 */
void LCD_init();
/*
 * Description:
 * send a command to the LCD
 */
void LCD_sendCommand(uint8 command);
/*
 * Description:
 * Display a char on the screen
 */
void LCD_displayCharacter(uint8 data);
/*
 * Description:
 * Display a string on the screen
 */
void LCD_displayString(uint8 *str);
/*
 * Description:
 * move the cursor to a certain location(row and column)
 */
void LCD_moveCursor(uint8 row,uint8 col);
/*
 * Description:
 * Display a string in a certain location (row and column)
 */
void LCD_displayStringRowColumn(uint8 row,uint8 col,const uint8* str);
/*
 * Description:
 * clear the screen
 */
void LCD_clearScreen();
/*
 * Description:
 * Display a decimal number on the screen
 */
void LCD_intgerToString(int data);

#endif /* LCD_H_ */
