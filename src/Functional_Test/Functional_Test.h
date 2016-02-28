/*
 * Functional_Test.h
 *
 *  Created on: Feb 18, 2016
 *      Author: cedrictoncanier
 */

#ifndef FUNCTIONAL_TEST_FUNCTIONAL_TEST_H_
#define FUNCTIONAL_TEST_FUNCTIONAL_TEST_H_


/*******************************************
*			  I N C L U D E 			   *
********************************************/
#include "../Functional_Test/Functional_Test.h"
#include "../Lib_Servo/Lib_Servo.h"

/*******************************************
*	        F U N C T I O N S   	       *
********************************************/
void Functional_Test_init();
void Functional_Test_PWM();
void Functional_Test_Sonar();
void servo_180();

#endif /* FUNCTIONAL_TEST_FUNCTIONAL_TEST_H_ */
