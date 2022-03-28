/************************************************************************************
 * Module: Timer
 *
 * Filename: timer.h
 *
 * Description: header file for the timer driver
 *
 * Created on: Oct 28, 2021
 *
 * Author: Mostafa Mahmoud
 ************************************************************************************/

#ifndef TIMER_H_
#define TIMER_H_
#include "std_types.h"
/*********************************************************************
 * 							Type Definitions
 *********************************************************************/
typedef enum
{
	NORMAL,CTC=2
}Timer_Mode;

typedef enum
{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024,EXTERNAL_FALLING,EXTERNAL_RAISING
}Timer_PreScaler;

typedef enum
{
	DISABLE_N,ENABLE_N
}Normal_mode_INT_Enable;

typedef enum
{
	DISABLE,ENABLE
}CTC_mode_INT_Enable;

typedef struct
{
	Timer_Mode mode;
	Timer_PreScaler prescaler;
	uint8 Initial_value;
	uint8 Compare_value;
	Normal_mode_INT_Enable N_INT;
	CTC_mode_INT_Enable CTC_INT;
}Timer_Config;

/*********************************************************************
 * 							Function Prototypes
 *********************************************************************/

/*
 * Description:
 * Function to initialize the timer module
 * 1- Set the timer mode
 * 2- Set the timer prescaler
 * 3- Set the timer initial value
 * 4- Set the timer output compare match value (CTC mode)
 */

void Timer_init(Timer_Config* config);

/*
 * Description:
 * Function to set the call back function
 */

void Timer_setCallBack(void(*a_ptr)(void));
/*
 * Description:
 * Function to de-initialize the timer
 */
void Timer_deInit();

#endif /* TIMER_H_ */
