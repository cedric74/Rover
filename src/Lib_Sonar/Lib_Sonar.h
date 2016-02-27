/*
 * Lib_Sonar.h
 *
 *  Created on: 2015-03-31
 *      Author: cedric
 */

#ifndef LIB_SONSAR_H_
#define LIB_SONSAR_H_

/*******************************************
*			  I N C L U D E 			   *
********************************************/
// Standard header files
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <math.h>

// Driver header file
#include "prussdrv.h"
#include <pruss_intc_mapping.h>

/*******************************************
*               D E F I N E                *
********************************************/
// READ_SONAR_PIN	P9_28	PRU
// WRITE_SONAR_PIN	p9_27	PRU


#define PRU_NUM  0
#define ADDEND1  0x0010F012u
#define ADDEND2  0x0000567Au
#define OFFSET_DDR  0x00001000
#define PRU_ADDR 	0x4A300000
#define SHAREDRAM_OFFSET 0x00012000
#define PRUSS0_SHARED_DATARAM    4
#define AM33XX
/*******************************************
*   T Y P E D E F   &  C O N S T A N T E   *
********************************************/
#define PROCESS_OK		0

// Formula to get the distance from the time of the sonar pulse feedback
#define TIME_2_DISTANCE(a)		( ( (a) * (0.0174) ) - (1.5602) )

/*******************************************
*	 G L O B A L   V A R I A B L E S  	   *
********************************************/
float fDistance;

/*******************************************
*	        F U N C T I O N S   	       *
********************************************/
void Lib_Sonar_Init();
int	 Lib_Sonar_Ping();

#endif /* LIB_SONSAR_H_ */
