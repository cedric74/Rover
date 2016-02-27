/*
 * Lib_Motor.c
 *
 *  Created on: Mar 4, 2015
 *      Author: cedric
 */

/*******************************************
*			  I N C L U D E 			   *
********************************************/
#include	"Lib_Motor.h"

/*******************************************
*               D E F I N E                *
********************************************/

/*******************************************
*   T Y P E D E F   &  C O N S T A N T E   *
********************************************/

/*******************************************
*	 G L O B A L   V A R I A B L E S  	   *
********************************************/

/*******************************************
*	        F U N C T I O N S   	       *
********************************************/
/*
 ============================================
 Function     : Lib_motor_control()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void Lib_motor_rover_U_Turn(){
	// Rotate 180 to The Left OR Right
	Lib_motor_control(ROTATE_LEFT);
	usleep(TIME_ROTATE_180);
	Lib_motor_control(STOP_MOVE);
}

/*
 ============================================
 Function     : Lib_motor_rover_Rotate()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void Lib_motor_rover_Rotate(eCtrlDirection direction, int iRotateDegre){
	Lib_motor_control(direction);
	usleep(iRotateDegre);
	Lib_motor_control(STOP_MOVE);
}


/*
 ============================================
 Function     : Lib_motor_control()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void Lib_motor_control(eCtrlDirection direction){
	eCmdMotor cmdMotorLeft = tabCtrlDirection[direction][MOTOR_LEFT];
	drv_motor_ctrl(cmdMotorLeft, MOTOR_LEFT);

	eCmdMotor cmdMotorRight = tabCtrlDirection[direction][MOTOR_RIGHT];
	drv_motor_ctrl(cmdMotorRight, MOTOR_RIGHT);

}

/*
 ============================================
 Function     : Lib_motor_init()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void Lib_motor_init(){

	// Init Motor LEFT
	drv_motor_init(MOTOR_LEFT);
	drv_motor_stop(MOTOR_LEFT);

	// Init Motor RIGHT
	drv_motor_init(MOTOR_RIGHT);
	drv_motor_stop(MOTOR_RIGHT);
}

/*
 ============================================
 Function     : drv_motor_init()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void drv_motor_ctrl(eCmdMotor cmd, eMotor Motor){
	switch(cmd){
		case  FRONT:
			//printf(" FRONT , Motor %d \n", Motor);
			drv_motor_front(Motor);
		break;
		case  REAR:
			//printf(" REAR , Motor %d \n", Motor);
			drv_motor_rear(Motor);
		break;
		case  STOP:
			//printf(" STOP , Motor %d \n", Motor);
			drv_motor_stop(Motor);
		break;

	}
}
/*
 ============================================
 Function     : drv_motor_init()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void drv_motor_init(eMotor Motor){
	beh_BBB_gpio_conf_dir(tabMotorPin[Motor][InA], OUTPUT);
	beh_BBB_gpio_conf_dir(tabMotorPin[Motor][InB], OUTPUT);
}

/*
 ============================================
 Function     : drv_motor_stop()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void drv_motor_stop(eMotor Motor){
	beh_BBB_gpio_WritePin(tabMotorPin[Motor][InA], tabCmdMotor[STOP][InA]);
	beh_BBB_gpio_WritePin(tabMotorPin[Motor][InB], tabCmdMotor[STOP][InB]);
}

/*
 ============================================
 Function     : drv_motor_front()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void drv_motor_front(eMotor Motor){
	beh_BBB_gpio_WritePin(tabMotorPin[Motor][InA], tabCmdMotor[FRONT][InA]);
	beh_BBB_gpio_WritePin(tabMotorPin[Motor][InB], tabCmdMotor[FRONT][InB]);
}

/*
 ============================================
 Function     : drv_motor_rear()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void drv_motor_rear(eMotor MotorToInit){
	beh_BBB_gpio_WritePin(tabMotorPin[MotorToInit][InA], tabCmdMotor[REAR][InA]);
	beh_BBB_gpio_WritePin(tabMotorPin[MotorToInit][InB], tabCmdMotor[REAR][InB]);
}
