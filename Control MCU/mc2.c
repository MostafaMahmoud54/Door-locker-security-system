/*******************************************************************************
 * Filename: mc2.c
 *
 * Description: Source file for the application of the control MCU
 *
 * Created on: Nov 2, 2021
 *
 * Author: Mostafa Mahmoud
 *******************************************************************************/
#include "uart.h"
#include "i2c.h"
#include "external_eeprom.h"
#include "timer.h"
#include "buzzer.h"
#include "Dc-motor.h"
#include "std_types.h"
#include "commands.h"
#include "functions.h"
#include <avr/io.h>
#include <avr/delay.h>

#define ERROR 0
#define SUCCESS 1
#define PASSWORDSIZE 5

/*******************************************************************************
 * 							Global Variables
 *******************************************************************************/
uint8 g_Password[5];
uint8 g_RePassword[5];
uint8 g_PasswordCheck[5];
uint8 g_EEPROMPassword[5];
uint8 g_check=0;
uint32 g_ticks=0;
uint8 g_error=0;
uint8 try[5];
uint8 checked=0;
uint8 selection;
uint8 newPassError=0;
uint8 flag=1;

/*******************************************************************************
 *							Function Definitions
 *******************************************************************************/

/*
 * Description:
 * Function responsible for receiving password form the other MCU
 * Inputs:
 * Pointer to array
 */
void Receive_Password(uint8 *str)
{
	uint8 i=0;
	while(i<PASSWORDSIZE)
	{
		str[i]=	UART_receiveByte();
		i++;
	}
}

/*
 * Decription:
 * Function responsible for receiving 2 passwords from the UI MCU
 */
void creatPassword()
{
	/*
	 * Synchronize with the other MCU
	 */
	while(UART_receiveByte()!=ready);
	UART_sendByte(MC2_READY);
	Receive_Password(g_Password);
	UART_sendByte(received);
	while(UART_receiveByte()!=REPASSWORD);
	UART_sendByte(send);
	Receive_Password(g_RePassword);
	/*
	 * sending receive confirmation
	 */
	UART_sendByte(received);
}

/*
 * Description:
 * Function responsible for comparing 2 passwords
 * Inputs:
 * 2 Pointer to string
 */
uint8 CheckPassword(uint8 *a_Password,uint8 *a_RePassword)
{
	uint8 i=0;
	while(i<PASSWORDSIZE)
	{
		if(a_Password[i] != a_RePassword[i])
		{
			return ERROR;
		}
		i++;
	}
	return SUCCESS;
}

/*
 * Description:
 * Function responsible for saving the password in the EEPROM
 */
void savePassword()
{
	for(uint8 i=0;i<PASSWORDSIZE;i++)
	{
		EEPROM_writeByte(0x0311+i,g_RePassword[i]);
		_delay_ms(100);
	}
}

/*
 * Description:
 * Function responsible for getting the password from the EEPROM
 */
void getPassword()
{
	for(uint8 i=0;i<PASSWORDSIZE;i++)
	{
		EEPROM_readByte(0x0311+i,g_EEPROMPassword+i);
	}
}

/*
 * Description:
 * Function responsible for comparing the entered password with the saved password
 * Output:
 *
 */
uint8 checkk()
{
	/*
	 * Synchronize with the other MCU
	 */
	while(UART_receiveByte()!=check);
	UART_sendByte(check);
	/*
	 * Receive the entered password
	 */
	Receive_Password(g_PasswordCheck);
	/*
	 * get the saved password from the EEPROM
	 */
	getPassword();
	/*
	 * get the compare result
	 */
	g_check=CheckPassword(g_PasswordCheck,g_EEPROMPassword);
	UART_sendByte(result);
	while(UART_receiveByte()!=result);
	/*
	 * Send the compare result
	 */
	UART_sendByte(g_check);
	return g_check;
}

/*
 * Description:
 * Function responsible for
 * 1-Displaying the main options
 * 2-Calling another function to check the entered password
 * 3-Call another function according to the choice
 */
void mainOptions()
{
	while(UART_receiveByte()!=start);
	UART_sendByte(start);

	while(UART_receiveByte()!=send);
	/*
	 * Receive the user selection
	 */
	selection=UART_receiveByte();
	checkk();
	flag=0;
	if (g_check==1)
	{
		if(selection==plus)
		{
			/*
			 * open the door based on the user selection
			 */
			openDoor();
		}
		else if (selection==minus)
		{
			/*
			 * Make a new password based on the user selection
			 */
			MC2_newPassword();
			flag=1;
		}
		g_check=0;
	}
	else
	{
		g_error++;
		while(UART_receiveByte()!=error);
		UART_sendByte(error);
		/*
		 * if the password is wrong ask the user for the password 3 successive times
		 */
		while(g_error<3)
		{
			if(checkk()==1)
			{
				if(selection==plus)
				{
					openDoor();
				}
				else if (selection==minus)
					/*
					 * if one of the 3 entries is right ask for new password
					 */
				{
					MC2_newPassword();
				}
				g_check=0;
				flag=1;
				break;
			}
			else
			{
				g_error++;
			}
		}
		/*
		 * Display error after 3 wrong password entries
		 */
		if(g_error==3)
		{
			errorRoutine();
		}
		g_error=0;
	}
}

/*
 * Description:
 * Function responsible for displaying error
 */
void errorRoutine()
{
	while(UART_receiveByte()!=error);
	UART_sendByte(error);
	TriggerBuzzer();
}

/*
 * Description:
 * Call back function responsible for triggering the buzzer for 15 seconds then turn it off
 */
void BuzzerCallBack()
{
	g_ticks++;
	if(g_ticks>1831)
	{
		g_ticks=0;
		Timer_deInit();
		BUZZER_off();
		flag=1;
	}
}

/*
 * Description:
 * Function responsible for starting the timer and turning the buzzer on
 */
void TriggerBuzzer()
{
	while(UART_receiveByte()!=startTimer);
	UART_sendByte(startTimer);
	Timer_setCallBack(BuzzerCallBack);
	BUZZER_on();
	startTimer0();
}

/*
 * Description:
 * Call back Function responsible for opening , closing and holding the door
 */
void motorcontrol()
{
	g_ticks++;
	/*
	 * Keep the motor on for 15 seconds then hold it
	 */
	if(g_ticks==458)
	{
		while(UART_receiveByte()!=HoldDoor);
		UART_sendByte(HoldDoor);
		DcMotor_OFF();
	}
	/*
	 * Hold the motor for 3 seconds
	 */
	else if(g_ticks==550)
	{
		while(UART_receiveByte()!=ClosingDoor);
		UART_sendByte(ClosingDoor);
		DcMotor_ON(ACW);
	}
	/*
	 * close the door for 15 seconds then turn it off
	 */
	else if(g_ticks==1008)
	{
		Timer_deInit();
		g_ticks=0;
		DcMotor_OFF();
		flag=1;
	}
}

/*
 * Description:
 * Function responsible for starting the timer and turning the motor on
 */
void openDoor()
{
	while(UART_receiveByte()!=startTimer);
	UART_sendByte(startTimer);
	Timer_setCallBack(motorcontrol);
	startTimer0();
	UART_sendByte(OpeningDoor);
	while(UART_receiveByte()!=OpeningDoor);
	DcMotor_ON(CW);
}

/*
 * Description:
 * Function responsible for starting the timer
 */
void startTimer0()
{
	Timer_Config config_t={NORMAL,F_CPU_1024,0,0,ENABLE_N,DISABLE};
	Timer_init(&config_t);
}

/*
 * Description:
 * Function responsible for making sure the user entered 2 matched passwords
 */
void MC2_newPassword()
{
	do
	{
		creatPassword();
		checked=CheckPassword(g_Password,g_RePassword);
		UART_sendByte(send);

		while(UART_receiveByte()!=send);

		UART_sendByte(checked);

	}while(checked==0);
	/*
	 * save the password in the EEPROM
	 */
	savePassword();
}
int main(void)
{
	/*
	 * Configure the i2c module with:
	 * 1-Address for slave mode
	 * 2-bit-rate=400000
	 */
	TWI_CONFIG config_i2c={0b00000001,400000};
	TWI_init(&config_i2c);

	/*
	 * Initialize the Dc-motor
	 */
	DcMotor_Init();
	/*
	 * Initialize the buzzer
	 */
	BUZZER_init();

	/*
	 * Initialize the UART module with:
	 * 1-number of bits per frame=8-bits
	 * 2-Parity disabled
	 * 3-one stop-bit
	 * 4-Baud-rate=9600
	 */
	Uart_ConfigType config={EIGHT_BIT,DISABLED,ONE_BIT,9600};
	UART_init(&config);
	/*
	 * Enable the global interrupts
	 */
	SREG|=1<<7;

	/*
	 * Make a new password for the system
	 */
	MC2_newPassword();
	while(1)
	{
		/*
		 * check if the flag=1 then the system finished its task(opening door or changing password)
		 * then back to the main menu
		 */
		if(flag==1)
			mainOptions();
	}
}
