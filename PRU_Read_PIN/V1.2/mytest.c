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

// Driver header file
#include "prussdrv.h"
#include <pruss_intc_mapping.h>

#include "beh_BBB_gpio.h"

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
	beh_BBB_gpio_init();
	
    unsigned int ret;
    tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;
    
    printf("\nINFO: Starting %s example.\r\n", "PRU_memAccess_DDR_PRUsharedRAM Version 1.1");
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
	
	printf("\tINFO: Initializing example.\r\n");
    //LOCAL_exampleInit(PRU_NUM);
    
	beh_BBB_gpio_conf_dir( P9_13, OUTPUT);
	beh_BBB_gpio_WritePin(P9_13, 0);
	sleep(1);
	beh_BBB_gpio_WritePin(P9_13, 1);
	beh_BBB_gpio_WritePin(P9_13, 1);
	beh_BBB_gpio_WritePin(P9_13, 1);
	beh_BBB_gpio_WritePin(P9_13, 1);
	beh_BBB_gpio_WritePin(P9_13, 1);
	beh_BBB_gpio_WritePin(P9_13, 1);
	beh_BBB_gpio_WritePin(P9_13, 0);
	
	//usleep(5);
	beh_BBB_gpio_conf_dir( P9_13, PIN_PULLDOWN);
	
    /* Execute example on PRU */
   // printf("\tINFO: Executing example.\r\n");
	
    prussdrv_exec_program (PRU_NUM, "./prucode.bin");
	uint32_t x[2] = {0};
	prussdrv_pru_clear_event (PRU_EVTOUT_0, PRU0_ARM_INTERRUPT);
	//signal(SIGINT, on_sigint);
  //while (is_sigint == 0)
  //{
		prussdrv_pru_wait_event (PRU_EVTOUT_0);
		read_words(x);
		printf("0x%08x\n", x[0]);
  //}


    /* Wait until PRU0 has finished execution */
    //printf("\tINFO: Waiting for HALT command.\r\n");
   // prussdrv_pru_wait_event (PRU_EVTOUT_0);
    printf("\tINFO: PRU completed transfer.\r\n");
    prussdrv_pru_clear_event (PRU_EVTOUT_0, PRU0_ARM_INTERRUPT);
	
    // test if program passed, final variables are good, and clean up
	printf("\tINFO: Try to read data.\r\n");
	//LOCAL_examplePassed(PRU_NUM);
    
    /* Disable PRU and close memory mapping*/
    prussdrv_pru_disable(PRU_NUM); 
    prussdrv_exit ();
	
	//read_words(x);
	
	//printf("0x%08x, 0x%08x\n", x[0], x[1]);
	
	//printf("0x%08x, 0x%08x\n", x[0], x[1]);
    
    return(0);
}

/******************************************************************************
* LOCAL_exampleInit Function                                                  *
******************************************************************************/
static int LOCAL_exampleInit ( unsigned short pruNum )
{
    //Initialize pointer to PRU data memory
    if (pruNum == 0)
    {
      prussdrv_map_prumem (PRU_ADDR+SHAREDRAM_OFFSET, &pruDataMem);
    }
    else if (pruNum == 1)
    {
      prussdrv_map_prumem (PRU_ADDR, &pruDataMem);
    }

    pdata=(char*) pruDataMem;   
    pruDataMem_int  = (unsigned int*) pruDataMem;
return(0);
}

/******************************************************************************
* LOCAL_exampleInit Function                                                  *
******************************************************************************/
static unsigned short LOCAL_examplePassed ( unsigned short pruNum )
{ 
	unsigned int *HPI_PWREMU_MGMT, *HPI_regaddr, *DDR_regaddr;
	unsigned int result_1, result_2, result_3;

    /* map the HPI memory */  
	/* Initialize memory pointer to start of External DDR memory */
	/* open the device */	
	
	
	int  mem_fd = open("/dev/mem",O_RDWR | O_SYNC);
	if (mem_fd < 0) {
		printf("Failed to open /dev/mem (%s)\n", strerror(errno));		
		return -1;	
	}		
	
    unsigned long* ddrMem =(unsigned long *) mmap(NULL, 0x10000, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, PRU_ADDR+SHAREDRAM_OFFSET);

    if (ddrMem == NULL) {
        printf("Failed to map the device (%s)\n", strerror(errno));
        close(mem_fd);
        return -1;
    }

	/* Read values from memory to confirm desired results from example */
	//HPI_PWREMU_MGMT = ddrMem + 0x00000004;
	//HPI_regaddr = ddrMem + 0x00000030;
	//DDR_regaddr = ddrMem + 0x00000004;

	//result_1 = *(unsigned long*) ddrMem;
	//result_2 = *(unsigned long*) HPI_regaddr;
	//result_3 =	*(unsigned long*) ddrMem;
	printf(" value : %X \n", ddrMem[0]);	
	printf(" value : %X \n", ddrMem[1]);	
	printf(" value : %X \n", ddrMem[2]);
	
	//printf(" value : %X \n", result_2);	
	//printf(" value : %X \n", result_3);	
	
	munmap(ddrMem, 0x0000FFFF);
	close(mem_fd);	
	
	// Report success or failure of the example
	return 0 ;
}


