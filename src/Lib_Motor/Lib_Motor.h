/*
 * Lib_Motor.h
 *
 *  Created on: Mar 4, 2015
 *      Author: cedric
 */

#ifndef LIB_MOTOR_H_
#define LIB_MOTOR_H_

/*******************************************
*			  I N C L U D E 			   *
********************************************/
#include "../Lib_Gpio/beh_BBB_gpio.h"

/*******************************************
*               D E F I N E                *
********************************************/

#define 	TIME_ROTATE_90		1700000	// in us, 1500000 Too Short, 1800000 Too Long, 1700000 OK
#define 	TIME_ROTATE_180		3800000 // in us, 4000000 Too Long,

/*******************************************
*   T Y P E D E F   &  C O N S T A N T E   *
********************************************/
typedef enum{
	MOTOR_LEFT	= 0,
	MOTOR_RIGHT	= 1,
}eMotor;

typedef enum{
	FRONT	= 0,
	REAR	= 1,
	STOP	= 3,
}eCmdMotor;

typedef enum{
	Pwm  = 0,
	InA  = 1,
	InB  = 2,
}eMotorPin;

typedef enum{
	ROTATE_RIGHT	= 0,
	ROTATE_LEFT		= 1,
	FORWARD			= 2,
	BACKWARD		= 3,
	STOP_MOVE		= 4,
	NO_CMD			=255,
}eCtrlDirection;


static const GPIO_Pins 	tabMotorPin[2][3]={
// 	        Motor,   PWM,     InA, 	  InB,
{/*	 MOTOR_LEFT*/	 P9_16,	P9_11,	P9_13 },
{/*	 MOTOR_RIGHT*/	 P9_18,	P9_15,  P9_12 },
};


static const int 	tabCmdMotor[3][3]={
// 	   Cmd, PWM,	 InA,	InB,

{/*	FRONT*/	  0,	HIGH,    LOW  },
{/*	 REAR*/	  0,	 LOW,	HIGH  },
{/*	 STOP*/	  0,	HIGH,	HIGH  },
};

static const eCtrlDirection 			tabCtrlDirection[][2]={
// 	    Direction,    Cmd Motor Left,   Cmd Motor right,

{/*	 ROTATE_LEFT*/	            REAR,			 FRONT  },
{/*	ROTATE_RIGHT*/	           FRONT,			  REAR  },
{/*	     FORWARD*/	           FRONT,			 FRONT  },
{/*	    BACKWARD*/	            REAR,			  REAR  },
{/*	   STOP_MOVE*/	            STOP,			  STOP  },

};


/*******************************************
*	 G L O B A L   V A R I A B L E S  	   *
********************************************/
eCtrlDirection	CmdDirection;

/*******************************************
*	        F U N C T I O N S   	       *
********************************************/
void 	Lib_motor_rover_Rotate(eCtrlDirection direction, int iRotateDegre);
void	Lib_motor_rover_U_Turn();


void 	drv_motor_ctrl(eCmdMotor cmd, eMotor Motor);
void 	drv_motor_init(eMotor Motor);
void 	drv_motor_stop(eMotor Motor);
void	drv_motor_rear(eMotor Motor);
void	drv_motor_front(eMotor Motor);

void 	Lib_motor_control(eCtrlDirection direction);
void 	Lib_motor_init();

#endif /* LIB_MOTOR_H_ */
