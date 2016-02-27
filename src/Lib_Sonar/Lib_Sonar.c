/*
 * Lib_Sonar.c
 *
 *  Created on: 2015-03-31
 *      Author: cedric
 */


/*******************************************
*			  I N C L U D E 			   *
********************************************/
#include "Lib_Sonar.h"

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
*	  S T A T I C   F U N C T I O N S   	*
********************************************/
static int Lib_Sonar_read_words_PRU(uint32_t x[2]);

/*******************************************
* F U N C T I O N S    D E F I N I T I O N S *
********************************************/
/*
 ============================================
 Function     : Lib_Sonar_Init()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void Lib_Sonar_Init(){

	// Init PRUSS
	system("echo  PRUIO > /sys/devices/bone_capemgr.9/slots");
	system("modprobe uio_pruss");
	printf( " Init PRU OK \n");

	// Init Variables Distance
	fDistance = 0;
}

/*
 ============================================
 Function     : Lib_Sonar_Ping()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
int Lib_Sonar_Ping(){
	printf( " Test Sonar Ping \n");

	// Launch Program myTest
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

	prussdrv_exec_program (PRU_NUM, "Lib_Sonar/./prucode.bin");

	prussdrv_pru_clear_event (PRU_EVTOUT_0, PRU0_ARM_INTERRUPT);

	/* Wait until PRU0 has finished execution */
	prussdrv_pru_wait_event (PRU_EVTOUT_0);

	// Clear Event
	printf("\tINFO: PRU completed transfer.\r\n");
	prussdrv_pru_clear_event (PRU_EVTOUT_0, PRU0_ARM_INTERRUPT);

	// Read Time Pulse From PRU
	Lib_Sonar_read_words_PRU(x);

	// Get The Distance With the Formula
	fDistance = TIME_2_DISTANCE(x[0]);

	/* Disable PRU and close memory mapping*/
	prussdrv_pru_disable(PRU_NUM);
	prussdrv_exit ();

	printf(" Time : %d us , Distance : %4.2f cm \n", x[0] , fDistance);

	return PROCESS_OK;
}


/*
 ============================================
 Function     : Lib_Sonar_read_words_PRU()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
static int Lib_Sonar_read_words_PRU(uint32_t x[2])
{
  static const size_t sharedram_offset = 2048;
  volatile uint32_t* p;

  prussdrv_map_prumem(4, (void**)&p);

  x[0] = p[sharedram_offset + 0];
  x[1] = p[sharedram_offset + 1];

  return 0;
}
