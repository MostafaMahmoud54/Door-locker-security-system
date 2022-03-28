/************************************************************************************
 * Device: Buzzer
 *
 * Filename: Buzzer.c
 *
 * Description: Source file for the Buzzer driver
 *
 * Created on: Oct 28, 2021
 *
 * Author: Mostafa Mahmoud
 ************************************************************************************/

#include "buzzer.h"
#include "gpio.h"
void BUZZER_init()
{
	GPIO_setupPinDirection(BUZZER_PORT,BUZZER_PIN,PIN_OUTPUT);
	GPIO_writePin(BUZZER_PORT,BUZZER_PIN,LOGIC_LOW);
}
void BUZZER_on()
{
	GPIO_writePin(BUZZER_PORT,BUZZER_PIN,LOGIC_HIGH);
}
void BUZZER_off()
{
	GPIO_writePin(BUZZER_PORT,BUZZER_PIN,LOGIC_LOW);
}
