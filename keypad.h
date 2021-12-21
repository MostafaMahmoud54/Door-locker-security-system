/*********************************************************************************************
 * Module: keypad
 *
 * Filename: keypad.c
 *
 * Description: Header file for the keypad driver
 *
 * Created on: Oct 4, 2021
 *
 * Author: Mostafa Mahmoud
 *********************************************************************************************/

#ifndef KEYPAD_H_
#define KEYPAD_H_
#include "std_types.h"
/*************************************************************************************
 * 									Definitions
 ************************************************************************************/

/*Keypad Configuration for number of rows and columns*/
#define KEYPAD_NUM_ROWS 4
#define KEYPAD_NUM_COlS 4

/*Keypad port configuration*/
#define KEYPAD_PORT PORTA_ID
#define KEYPAD_FIRST_ROW_PIND_ID PIN0_ID
#define KEYPAD_FIRST_COL_PIND_ID PIN4_ID

/* Keypad button logic configurations */
#define KEYPAD_BUTTON_PRESSED            LOGIC_LOW
#define KEYPAD_BUTTON_RELEASED           LOGIC_HIGH

/*************************************************************************************
 * 								Function Prototypes
 ************************************************************************************/

/*
 * Description: Get keypad pressed button
 */
uint8 KEYPAD_getPressedKey(void);


#endif /* KEYPAD_H_ */
