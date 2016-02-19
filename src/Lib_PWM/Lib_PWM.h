/*
 * Lib_Sonar.h
 *
 *  Created on: 2015-03-31
 *      Author: cedric
 */

#ifndef LIB_PWM_H_
#define LIB_PWM_H_

/*******************************************
*			  I N C L U D E 			   *
********************************************/

/*******************************************
*               D E F I N E                *
********************************************/
/*******************************************
*			  I N C L U D E 			   *
********************************************/
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../Lib_Gpio/beh_BBB_gpio.h"

/*******************************************
*               D E F I N E                *
********************************************/
#define 	BUFFER_SIZE 	128

#define		PATH_P9_14			"/sys/devices/ocp.3/pwm_test_P9_14."

#define 	PATH_POLARITY  	"/polarity"
#define 	PATH_PERIOD  	"/period"
#define 	PATH_DUTY    	"/duty"
#define     PATH_RUN		"/run"

#define		Offset_11			"11"
#define		Offset_12			"12"
#define		Offset_13			"13"
#define		Offset_14			"14"
#define		Offset_15			"15"
#define		Offset_16			"16"
#define		Offset_17			"17"
#define		Offset_18			"18"
#define		Offset_19			"19"
#define		Offset_20			"20"
#define		Offset_21			"21"
#define		Offset_22			"22"




#define 	NO_ERROR_PWM	 0
#define 	ERROR_PWM_OPEN	-1
#define 	ERROR_PWM_WRITE	-2

/*******************************************
*   T Y P E D E F   &  C O N S T A N T E   *
********************************************/

/*******************************************
*	 G L O B A L   V A R I A B L E S  	   *
********************************************/
static const int OFFSET_11_22 = 22;

/*******************************************
*	        F U N C T I O N S   	       *
********************************************/
int Lib_pwm_init();
int Lib_pwm_control(int iPeriod, int iDuty );
int Lib_pwm_stop();
int Lib_pwm_start();

#endif /* LIB_PWM_H_ */
