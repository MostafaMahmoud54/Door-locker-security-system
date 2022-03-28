/*
 * Dc-motor.h
 *
 *  Created on: Oct 11, 2021
 *      Author: Mostafa Mahmoud
 */

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_
/************************************************************************************
 *                                Definitions
 ************************************************************************************/
#define Dc_MOTOR_PIN0 PIN6_ID
#define Dc_MOTOR_PIN1 PIN7_ID
#define Dc_MOTOR_PORT PORTD_ID
#define Dc_MOTOR_ENABLE_PIN PIN5_ID
/************************************************************************************
 * 								 Types Declaration
 ************************************************************************************/
typedef enum
{
	CW,ACW
}DcMotor_Direction;

/************************************************************************************
 * 						Function Prototypes
 ************************************************************************************/
/*
 * Function to initialize the Dc-motor pins and stop the motor at the beginning
 */
void DcMotor_Init(void);
/*
 * Description:
 * Function to set the direction of the Dc-motor with maximum speed
 */
void DcMotor_ON(DcMotor_Direction direction);
/*
 * Description:
 * Function to turn OFF the motor
 */
void DcMotor_OFF();
#endif /* DC_MOTOR_H_ */
