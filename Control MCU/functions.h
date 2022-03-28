/*
 * functions.h
 *
 *  Created on: Nov 8, 2021
 *      Author: Mostafa Mahmoud
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

void Receive_Password(uint8 *str);
void send_password(uint8 *str);
void creatPassword();
uint8 CheckPassword(uint8 *a_Password,uint8 *a_RePassword);
void savePassword();
void getPassword();
uint8 checkk();
void mainOptions();
void errorRoutine();
void BuzzerCallBack();
void TriggerBuzzer();
void motorcontrol();
void openDoor();
void startTimer0();
void MC2_newPassword();


#endif /* FUNCTIONS_H_ */
