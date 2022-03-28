/************************************************************************************
 * Module: keypad
 *
 * Filename: keypad.c
 *
 * Description: source file for the keypad driver
 *
 * Created on: Oct 4, 2021
 *
 * Author: Mostafa Mahmoud
 ************************************************************************************/

#include "gpio.h"
#include "keypad.h"
#if(KEYPAD_NUM_COlS==3)
static uint8 KEYPAD_4x3_adjustKeyNumber(uint8 button_number);
#elif(KEYPAD_NUM_COlS==4)
static uint8 KEYPAD_4x4_adjustKeyNumber(uint8 button_number);
#endif
/*************************************************************************************
 * 								Function Definitions
 ************************************************************************************/
uint8 KEYPAD_getPressedKey(void)
{
	uint8 row,col,KEYPAD_PORT_VALUE=0;
	while(1)
	{
		for(col=0;col<KEYPAD_NUM_COlS;col++) /*loop for columns*/
		{
			/*
			 * Each time setup the direction for all keypad port as input pins,
			 * except this column will be output pin
			 */
			GPIO_setupPortDirection(KEYPAD_PORT,PORT_INPUT);
			GPIO_setupPinDirection(KEYPAD_PORT,KEYPAD_FIRST_COL_PIND_ID+col,PIN_OUTPUT);

			#if(KEYPAD_BUTTON_PRESSED==LOGIC_LOW)
			/*clear the column output pin and set the rest pin values*/
			KEYPAD_PORT_VALUE=~(1<<(KEYPAD_FIRST_COL_PIND_ID+col));

			#else
			/*clear the column output pin and set the rest pin values*/
			KEYPAD_PORT_VALUE=(1<<KEYPAD_FIRST_COL_PIND_ID+col);
			#endif

			GPIO_writePort(KEYPAD_PORT,KEYPAD_PORT_VALUE);
			for(row=0;row<KEYPAD_NUM_ROWS;row++) /*loop for rows*/
			{

				if(GPIO_readPin(KEYPAD_PORT,row+KEYPAD_FIRST_ROW_PIND_ID)==KEYPAD_BUTTON_PRESSED)
				{
					#if(KEYPAD_NUM_COlS==4)
						return KEYPAD_4x4_adjustKeyNumber((row*KEYPAD_NUM_COlS)+col+1);
					#elif (KEYPAD_NUM_COlS==3)
						return KEYPAD_4x3_adjustKeyNumber((row*KEYPAD_NUM_COlS)+col+1);
					#endif
				}
			}
		}
	}
}

#if (KEYPAD_NUM_COlS==3)

/*
 * Description: Update the keypad pressed button with the correct one in the 4x3 shape
 */
static uint8 KEYPAD_4x3_adjustKeyNumber(uint8 button_number)
{
	uint8 keypad_button=0;
	switch(button_number)
	{
	case 10:
		keypad_button='*'; /*asci code of '*' */
		break;
	case 11:
		keypad_button=0;
		break;
	case 12:
		keypad_button='#'; /*asci code of '#' */
		break;
	default:
		keypad_button=button_number;
		break;
	}
	return keypad_button;
}

#elif (KEYPAD_NUM_COlS==4)

/*
 * Description: Update the keypad pressed button with the correct one in the 4x4 shape
 */

static uint8 KEYPAD_4x4_adjustKeyNumber(uint8 button_number)
{
	uint8 keypad_button=0;
	switch(button_number)
	{
	case 1:
		keypad_button=7;
		break;
	case 2:
		keypad_button=8;
		break;
	case 3:
		keypad_button=9;
		break;
	case 4:
		keypad_button='%'; /*asci code of '%' */
		break;
	case 5:
		keypad_button=4;
		break;
	case 6:
		keypad_button=5;
		break;
	case 7:
		keypad_button=6;
		break;
	case 8:
		keypad_button='*'; /*asci code of '*' */
		break;
	case 9:
		keypad_button=1;
		break;
	case 10:
		keypad_button=2;
		break;
	case 11:
		keypad_button=3;
		break;
	case 12:
		keypad_button='-'; /*asci code of '-' */
		break;
	case 13:
		keypad_button = 13; /*asci code of enter */
		break;
	case 14:
		keypad_button = 0;
		break;
	case 15:
		keypad_button = '='; /*asci code of '=' */
		break;
	case 16:
		keypad_button = '+'; /*asci code of '+' */
		break;
	default:
		keypad_button = button_number;
		break;
	}
	return keypad_button;
}
#endif
