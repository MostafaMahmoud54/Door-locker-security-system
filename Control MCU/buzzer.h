/************************************************************************************
 * Device: Buzzer
 *
 * Filename: Buzzer.h
 *
 * Description: Header file for the Buzzer driver
 *
 * Created on: Oct 28, 2021
 *
 * Author: Mostafa Mahmoud
 ************************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_

#define BUZZER_PORT PORTC_ID
#define BUZZER_PIN PIN3_ID

void BUZZER_init();
void BUZZER_on();
void BUZZER_off();

#endif /* BUZZER_H_ */
