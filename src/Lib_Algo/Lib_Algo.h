/*
 * Lib_Algo.h
 *
 *  Created on: Apr 9, 2015
 *      Author: cedric.toncanier
 */

#ifndef SRC_LIB_ALGO_H_
#define SRC_LIB_ALGO_H_

/*******************************************
*			  I N C L U D E 			   *
********************************************/
#include 	"../Lib_Motor/Lib_Motor.h"
//#include 	"../Lib_Sonar/Lib_Sonar.h"
#include	"../Lib_Servo/Lib_Servo.h"

#include  <pthread.h>
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
void 	Lib_Algo_Stop();
void 	Lib_Algo_Start();
void 	Lib_Algo_Init();


#endif /* SRC_LIB_ALGO_H_ */
