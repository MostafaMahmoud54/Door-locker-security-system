/*
 * functions.h
 *
 *  Created on: Nov 8, 2021
 *      Author: Mostafa Mahmoud
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_


void Enter_password(uint8* a_password);
void creat_Password();
void send_password(uint8 *str);
void Receive_Password(uint8 *str);
uint8 CheckPassword();
void changePassword();
void main_options();
void motorcontrol();
void openDoor();
void ErrorCallBack();
void displayError();
void startTimer0();
void NewPassword();
#endif /* FUNCTIONS_H_ */
