/*
 * ============================================================================
 * Copyright (c) Texas Instruments Inc 2010-12
 * Modified by TekuConcept 2014-6-16
 *
 * Use of this software is controlled by the terms and conditions found in the
 * license agreement under which this software has been supplied or provided.
 * ============================================================================
 *
 *
 *
 * This program demonstraits how one can read from a pin. The program will wait
 * until pin P9.28 is HIGH. Then when it is HIGH, pin P9.27 is turned off and
 * the program is completed.
 *
 */

/******************************************************************************
* Include Files                                                               *
******************************************************************************/

// Standard header files
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>    /* signal name macros, and the signal() prototype */

#include <math.h>
// Driver header file
#include "prussdrv.h"
#include <pruss_intc_mapping.h>

/******************************************************************************
* Local Macro Declarations                                                    *
******************************************************************************/
#define PRU_NUM  0
#define ADDEND1  0x0010F012u
#define ADDEND2  0x0000567Au
#define OFFSET_DDR  0x00001000 
#define PRU_ADDR 	0x4A300000
#define SHAREDRAM_OFFSET 0x00012000
#define PRUSS0_SHARED_DATARAM    4
#define AM33XX

static void *pruDataMem;
static unsigned int *pruDataMem_int;
static char *pdata;
static int LOCAL_exampleInit ( unsigned short pruNum );
static unsigned short LOCAL_examplePassed ( unsigned short pruNum );

/******************************************************************************
* Global Function Definitions                                                 *
******************************************************************************/
/* host pru shared memory */
 
static int read_words(uint32_t x[2])
{
  static const size_t sharedram_offset = 2048;
  volatile uint32_t* p;
 
  prussdrv_map_prumem(4, (void**)&p);
 
  x[0] = p[sharedram_offset + 0];
  x[1] = p[sharedram_offset + 1];
 
  return 0;
}

/* sigint handler */

static volatile unsigned int is_sigint = 0;

static void on_sigint(int x)
{
  is_sigint = 1;
}

int main (void)
{
	// Declarations Variables
	uint32_t x[2] = {0};
	
	// Instructions
	
    unsigned int ret;
    tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;
    
    printf("\nINFO: Starting %s example.\r\n", "PRU_DRIVER_SONAR V 1.4");
    /* Initialize the PRU */
    prussdrv_init ();		
    
    /* Open PRU Interrupt */
    ret = prussdrv_open(PRU_EVTOUT_0);
    if (ret)
    {
        printf("prussdrv_open open failed\n");
        return (ret);
    }
    
    /* Get the interrupt initialized */
    prussdrv_pruintc_init(&pruss_intc_initdata);
	
    /* Execute example on PRU */
    printf("\tINFO: Executing example.\r\n");
	
    prussdrv_exec_program (PRU_NUM, "./prucode.bin");

	prussdrv_pru_clear_event (PRU_EVTOUT_0, PRU0_ARM_INTERRUPT);
	/* Wait until PRU0 has finished execution */
	prussdrv_pru_wait_event (PRU_EVTOUT_0);
	read_words(x);	
	float fDistance =  (x[0] * (0.0174)) - (1.5602);
    printf("\tINFO: PRU completed transfer.\r\n");
    prussdrv_pru_clear_event (PRU_EVTOUT_0, PRU0_ARM_INTERRUPT);
	    
    /* Disable PRU and close memory mapping*/
    prussdrv_pru_disable(PRU_NUM); 
    prussdrv_exit ();
	 
	 
	 printf(" Time : %d us , Distance : %4.2f cm \n", x[0] , fDistance);
	 
    return(0);
}

