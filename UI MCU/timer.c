/************************************************************************************
 * Module: Timer
 *
 * Filename: timer.c
 *
 * Description: source file for the timer driver
 *
 * Created on: Oct 28, 2021
 *
 * Author: Mostafa Mahmoud
 ************************************************************************************/

#include "timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>
/******************************************************************************
 * 								Global Variables
 ******************************************************************************/
static volatile void (*g_callBackPtr)(void) = NULL_PTR;
/******************************************************************************
 * 								Function Definitions
 ******************************************************************************/
/*
 * Description:
 * Function to initialize the timer module
 * 1- Set the timer mode
 * 2- Set the timer prescaler
 * 3- Set the timer initial value
 * 4- Set the timer output compare match value (CTC mode)
 */
void Timer_init(Timer_Config* config)
{
	/*Configure the timer to work in a non-PWM mode*/
	TCCR0=(1<<FOC0);

	/*Configure the timer clock prescaler*/
	TCCR0=(TCCR0 & 0xF8) | (config->prescaler & 0x07);

	/*Set timer initial value*/
	TCNT0=config->Initial_value;

	/*Configure the timer to work in the Normal mode:
	 * WGM00=0 & WGM01=0
	 * COM00=0 & COM01=0
	 * Enable Timer Overflow Interrupt TOIE0=1
	 */
	TIMSK|=config->N_INT & 0x01;

	/*Configure the timer to work in the Normal mode:
	 * WGM00=0 & WGM01=1
	 * COM00=0 & COM01=0
	 * Set timer compare match value OCR0
	 * Enable Timer output compare match Interrupt OCIE0=1
	 */
	TCCR0=(TCCR0 & 0xF7)|((config->mode & 0x02)<<2);
	OCR0=config->Compare_value;
	TIMSK|=(config->CTC_INT & 0x01)<<1;
}
/*
 * Description:
 * Function to set the call back function
 */
void Timer_setCallBack(void(*a_ptr)(void))
{
	g_callBackPtr=a_ptr;
}
/*
 * Description:
 * Function to de-initialize the timer
 */
void Timer_deInit()
{
	TCCR0=0;
	TCNT0=0;
}
/******************************************************************************
 * 							Interrupt service routines
 ******************************************************************************/
/*ISR for the normal mode*/
ISR(TIMER0_OVF_vect)
{
	g_callBackPtr();
}
/*ISR for the compare match mode*/
ISR(TIMER0_COMP_vect)
{
	g_callBackPtr();
}
