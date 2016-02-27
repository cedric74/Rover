// prucode.p, Version Cedric Toncanier

.origin 0
.entrypoint START

#include "prucode.hp"

//#define MAGIC_NUMBER        0xbabe7175      // Magic number  
#define MAGIC_NUMBER        0xBABE      // Magic number  

START:

    // Enable OCP master port
    LBCO      r0, CONST_PRUCFG, 4, 4
    CLR       r0, r0, 4         // Clear SYSCFG[STANDBY_INIT] to enable OCP master port
    SBCO      r0, CONST_PRUCFG, 4, 4

    // =============================================================
    // set pin to high
//    SET r30.t5
//
//    // wait till a 1 is read
//    WBS r31.t3
//	
//	// wait till a 0 is read
//	WBC r31.t3
//
//    // clear pin to confirm read
//    CLR r30.t5
	
    // =============================================================
//	MOV  r0, 0                // Load the dataram address into r0
//    LBBO r2, r0, 0, 4                   // Load the ddr_addr from the first adress in the PRU0 DRAM
// 
//    MOV r1, MAGIC_NUMBER                // Place the magic number into the register
//    SBCO r1, r2, 0, 4                   // Write the magic number into DDR, addr is now in r2
	

	// prepare PRU shared memory access
MOV r0, 0x000000120
MOV r1, CTPPR_0
ST32 r0, r1
 
MOV r0, 0x00100000
MOV r1, CTPPR_1
ST32 r0, r1

MOV  r4, MAGIC_NUMBER                // Place the magic number into the register
SBCO r4, CONST_PRUSHAREDRAM, 0, 8



    // wait till a 1 is read
//   WBS r31.t3
//	
//	// wait till a 0 is read
//	WBC r31.t3
//

    // Send notification to Host for program completion
    MOV r31.b0, PRU0_ARM_INTERRUPT+16

    HALT
    
