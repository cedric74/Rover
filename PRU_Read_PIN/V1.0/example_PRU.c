//--- trunk/example_apps/PRU_access_const_table/PRU_access_const_table.c	2010/10/19 15:57:58	2
//+++ trunk/example_apps/PRU_access_const_table/PRU_access_const_table.c	2010/11/08 22:20:07	4
//@@ -95,12 +95,12 @@
int main (void)
{
    unsigned int ret;
	tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;
    printf("\nINFO: Starting %s example.\r\n", "PRU_access_const_table");
    /* Initialize the PRU */
	prussdrv_init ();		
	
    /* Open PRU Interrupt */
    ret = prussdrv_open(PRU_EVTOUT_0);
    if (ret)
        printf("prussdrv_open open failed\n");
        return (ret);
    }
	
	/* Get the interrupt initialized */
    prussdrv_pruintc_init(&pruss_intc_initdata);

	/* Initialize example */
	printf("\tINFO: Initializing example.\r\n");
    LOCAL_exampleInit(PRU_NUM);

    /* Execute example on PRU */
    printf("\tINFO: Executing example.\r\n");
    prussdrv_exec_program (PRU_NUM, "./PRU_access_const_table.bin");

	/* Wait until PRU has finished execution */
	printf("\tINFO: Waiting for HALT command.\r\n");
    prussdrv_pru_wait_event (PRU_EVTOUT_0);
	printf("\tINFO: PRU completed transfer.\r\n");
    prussdrv_pru_clear_event (PRU0_ARM_INTERRUPT);	

	/* Check if example passed */
	if ( LOCAL_examplePassed(PRU_NUM) )
	{
    	printf("Example executed succesfully.\r\n");
  	}
	else
	{
    	printf("Example failed.\r\n");
  	}
   
	/* Disable PRU and close memory mapping*/
	prussdrv_pru_disable(PRU_NUM); 
    prussdrv_exit ();
	munmap(ddrMem, 0x0FFFFFFF);
	munmap(HPI_0_regs, 0x00003000);
	close(mem_fd);
   
    return(0);
}


static int LOCAL_exampleInit ( unsigned short pruNum )
{
    /* Initialize pointer to PRU data memory */
    if (pruNum == 0)
    {
       prussdrv_map_prumem (PRUSS0_PRU0_DATARAM, &pruDataMem);
    }
    else if (pruNum == 1)
    {
       prussdrv_map_prumem (PRUSS0_PRU1_DATARAM, &pruDataMem);
    }  
    pruDataMem_int = (unsigned int*) pruDataMem;

    /* Set the PRU to do PSC state transition for HPI */
    pruDataMem_int[0] = 1;
    pruDataMem_int[1] = ((PSC_1 				  & 0xFF)<< 24 ) |
                    ((0 					  & 0xFF)<< 16 ) |	
                    ((PSC_HPI 			  	  & 0xFF)<<  8 ) |
                    ((PSC_MDCTL_NEXT_ENABLE   & 0xFF)<<  0 );

    /* Execute PSC configuration code on PRU */
    printf("\tINFO: Executing PSC configuration code .\r\n"); 
    prussdrv_exec_program (PRU_NUM, "./PRU_pscConfig.bin");

    /* Wait until PRU has finished execution */
	printf("\tINFO: Waiting for HALT command.\r\n");
    prussdrv_pru_wait_event (PRU_EVTOUT_0);
    printf("\tINFO: PRU completed transfer.\r\n");
    prussdrv_pru_clear_event (PRU0_ARM_INTERRUPT);				
	
	
	/* Initialize memory pointer to start of External DDR memory */
	/* open the device */	
	mem_fd = open("/dev/mem", O_RDWR);	
	if (mem_fd < 0) {
		printf("Failed to open /dev/mem (%s)\n", strerror(errno));		
		return -1;	
	}		

	/* map the external DDR memory */    
	ddrMem = mmap(0, 0x0FFFFFFF, PROT_WRITE | PROT_READ, MAP_SHARED, mem_fd, 0xC0000000);    
	if (ddrMem == NULL) {        
		printf("Failed to map the device (%s)\n", strerror(errno));        
		close(mem_fd);        
		return -1;    
	}			   
			
	/* Load the 1st DDR memory location */
	*(unsigned long*) ddrMem = ADDEND1;
    
    

    return(0);
}

static unsigned short LOCAL_examplePassed ( unsigned short pruNum )
{ 
	unsigned int *HPI_PWREMU_MGMT, *HPI_regaddr, *DDR_regaddr;
	unsigned int result_1, result_2, result_3;

    /* map the HPI memory */  
    HPI_0_regs = mmap(0, 0x00003000, PROT_WRITE | PROT_READ, MAP_SHARED, mem_fd, 0x01E10000);
    if (HPI_0_regs == NULL) {
        printf("Failed to map the device (%s)\n", strerror(errno));
        close(mem_fd);
        return -1;
    }

	/* Read values from memory to confirm desired results from example */
	HPI_PWREMU_MGMT = HPI_0_regs + 0x00000004;
	HPI_regaddr = HPI_0_regs + 0x00000030;
	DDR_regaddr = ddrMem + 0x00000004;

	result_1 = *(unsigned long*) HPI_PWREMU_MGMT;
	result_2 = *(unsigned long*) HPI_regaddr;
	result_3 =	*(unsigned long*) DDR_regaddr;
            printf("Failed to map the device (%s)\n", strerror(errno));
            close(mem_fd);
            return -1;
        }
	// Report success or failure of the example
	return ((result_1 == ADDEND1) && (result_2 == result_3)) ;
}



