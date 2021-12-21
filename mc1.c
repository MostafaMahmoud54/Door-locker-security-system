/*******************************************************************************
 * Filename: mc1.c
 *
 * Description: Source file for the application of the user interface MCU
 *
 * Created on: Nov 2, 2021
 *
 * Author: Mostafa Mahmoud
 *******************************************************************************/
#include "lcd.h"
#include "keypad.h"
#include "timer.h"
#include "uart.h"
#include "std_types.h"
#include "commands.h"
#include "functions.h"
#include <avr/delay.h>
#include <avr/io.h>

#define F_CPU 8000000
#define PASSWORDSIZE 5
/*******************************************************************************
 * 							Global Variables
 *******************************************************************************/
uint8 g_Password[5];
uint8 g_RePassword[5];
uint8 g_PasswordCheck[5];
uint8 g_check=0;
uint8 g_error=0;
uint32 g_ticks=0;
uint8 checked=0;
uint8 flag=1;
uint8 flag2=1;
uint8 key;
/*******************************************************************************
 *							Function Definitions
 *******************************************************************************/

/*
 * Description:
 * Function to get password from the keypad
 */
void Enter_password(uint8* a_password)
{
	uint8 i=0;
	uint8 key;
	LCD_moveCursor(1,0);
	for(i=0;i<PASSWORDSIZE+1;i++)
	{
		key=KEYPAD_getPressedKey();
		_delay_ms(700);
		if(key >=0 && key<=9)
		{
			a_password[i]=key;
			LCD_displayString("*");
		}
		else
		{
			_delay_ms(1000);
			break;
		}
		_delay_ms(1000);
	}
}

/*
 * Description:
 * Function responsible for getting 2 matched passwords from the user and send them to the other MCU
 */
void creat_Password()
{
	LCD_clearScreen();
	LCD_displayString("Enter New pass:");
	Enter_password(g_Password);
	/*
	 * Synchronize the 2 MCU'S
	 */
	UART_sendByte(ready);
	while(UART_receiveByte() != MC2_READY){};
	/*
	 * send the password
	 */
	send_password(g_Password);
	/*
	 * wait for the other MCU to confirm receiving
	 */
	while(UART_receiveByte() != received){};
	LCD_clearScreen();
	LCD_displayString("Re-Enter pass:");
	Enter_password(g_RePassword);
	UART_sendByte(REPASSWORD);
	while(UART_receiveByte() != send){};
	send_password(g_RePassword);
	/*
	 * wait for the other MCU to confirm receiving
	 */
	while(UART_receiveByte() != received){};
}

/*
 * Description:
 * Function to send the password to the other MCU
 */
void send_password(uint8 *str)
{
	for(uint8 i=0;i<PASSWORDSIZE;i++)
	{
		UART_sendByte(str[i]);
	}
}

/*
 * Description:
 * Check the equality of the password entered by the user and the saved password
 */
uint8 CheckPassword()
{
	LCD_clearScreen();
	LCD_displayString("Please Enter pass:");
	/*
	 * take the password from the user
	 */
	Enter_password(g_PasswordCheck);
	UART_sendByte(check);
	while(UART_receiveByte()!=check);
	/*
	 * send the password to the other MCU
	 */
	send_password(g_PasswordCheck);
	/*
	 * wait for the comparison result
	 */
	while(UART_receiveByte()!=result);
	UART_sendByte(result);
	/*
	 * get the result
	 */
	g_check=UART_receiveByte();
	/*
	 * return 1 if matched , 0 otherwise
	 */
	return g_check;
}

/*
 * Description:
 * Function responsible for:
 * 1-Displaying the main options
 * 2-Calling another function to check the entered password
 * 3-Call another function according to the choice
 */
void main_options()
{
	UART_sendByte(start);
	while(UART_receiveByte()!=start);

	LCD_clearScreen();
	LCD_displayString("+: open door");
	LCD_moveCursor(1,0);
	LCD_displayString("-: change password");
	/*
	 * take the choice from the user
	 */
	key=KEYPAD_getPressedKey();
	_delay_ms(1000);
	if(key=='+')
	{
		/*
		 * Synchronize with the other MCU
		 */
		UART_sendByte(send);
		UART_sendByte(plus);
		flag=0;
		if(CheckPassword()==1)
		{
			openDoor();
			g_check=0;
		}
		else
		{
			g_error++;
			/*
			 * Synchronize with the other MCU
			 */
			UART_sendByte(error);
			while(UART_receiveByte()!=error);
			/*
			 * if the password is wrong ask the user for the password 3 successive times
			 */
			while(g_error<3)
			{
				if(CheckPassword()==1)
				{
					openDoor();
					g_check=0;
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
				UART_sendByte(error);
				while(UART_receiveByte()!=error);
				displayError();
			}
			g_error=0;
		}
	}
	else if(key=='-')
	{
		flag=0;
		/*
		 * Synchronize with the other MCU
		 */
		UART_sendByte(send);
		UART_sendByte(minus);
		if(CheckPassword()==1)
		{
			NewPassword();
			flag=1;
			g_check=0;
		}
		else
		{
			g_error++;
			UART_sendByte(error);
			while(UART_receiveByte()!=error);
			/*
			 * if the password is wrong ask the user for the password 3 successive times
			 */
			while(g_error<3)
			{
				/*
				 * if one of the 3 entries is right ask for new password
				 */
				if(CheckPassword()==1)
				{
					NewPassword();
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
				UART_sendByte(error);
				while(UART_receiveByte()!=error);
				displayError();
			}
			g_error=0;
		}
	}
}

/*
 * Call back function for controlling the door
 */
void motorcontrol()
{
	g_ticks++;
	/*
	 * count 15 seconds opening the door then hold it
	 */
	if(g_ticks==458)
	{
		UART_sendByte(HoldDoor);
		while(UART_receiveByte()!=HoldDoor);
		LCD_clearScreen();
		LCD_displayString("Door is Open");
	}
	/*
	 * hold the door for 3 seconds
	 */
	else if(g_ticks==550)
	{
		UART_sendByte(ClosingDoor);
		while(UART_receiveByte()!=ClosingDoor);
		LCD_clearScreen();
		LCD_displayString("Closing Door");
	}
	/*
	 * count 15 seconds closing the door then de-initialize the timer
	 */
	else if(g_ticks==1008)
	{
		Timer_deInit();
		g_ticks=0;
		flag=1;
	}
}

/*
 * Description:
 * Function responsible for opening the door
 */
void openDoor()
{
	/*
	 * Synchronization with the other MCU to start opening the door
	 */
	UART_sendByte(startTimer);
	while(UART_receiveByte()!=startTimer);
	/*
	 * set the timer call back function
	 */
	Timer_setCallBack(motorcontrol);
	startTimer0();
	while(UART_receiveByte()!=OpeningDoor);
	UART_sendByte(OpeningDoor);
	LCD_clearScreen();
	LCD_displayString("Opening Door");
}

/*
 * Description:
 * Call back function responsible for erasing the error message and de-initialize the timer after one minute
 */
void ErrorCallBack()
{
	g_ticks++;
	if(g_ticks>1831)
	{
		g_ticks=0;
		Timer_deInit();
		LCD_clearScreen();
		flag=1;
	}
}

/*
 *Description:
 *Function Responsible for displaying error message
 */
void displayError()
{
	LCD_clearScreen();
	LCD_displayString("Wrong Password!!");
	/*
	 * Synchronize with the other MCU
	 */
	UART_sendByte(startTimer);
	while(UART_receiveByte()!=startTimer);
	/*
	 * Set the timer call back function
	 */
	Timer_setCallBack(ErrorCallBack);
	startTimer0();
}

/*
 * Function responsible for initializing the timer module
 */
void startTimer0()
{
	Timer_Config config_t={NORMAL,F_CPU_1024,0,0,ENABLE_N,DISABLE};
	Timer_init(&config_t);
}

/*
 * Description:
 * Function responsible for getting 2 matched passwords from the user
 */
void NewPassword()
{
	do
	{
		/*
		 * take 2 passwords from the user
		 */
		creat_Password();

		/*
		 * Synchronize with the other MCU
		 */
		while(UART_receiveByte()!=send);
		UART_sendByte(send);

		/*
		 * Receive the result of password checking from the other MCU
		 */
		checked=UART_receiveByte();

	}while(checked==0);
}

int main(void)
{
	/*
	 * Initialize the LCD device
	 */
	LCD_init();

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
	NewPassword();
	while(1)
	{
		/*
		 * check if the flag=1 then the system finished its task(opening door or changing password)
		 * then back to the main menu
		 */
		if(flag==1)
			main_options();
	}
}
