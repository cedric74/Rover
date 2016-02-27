// prucode.p, Version Cedric Toncanier
// Version V 1.1, 2015-04-06

.origin 0
.entrypoint START

#include "prucode.hp"


START:

    // Enable OCP master port
    LBCO      r0, CONST_PRUCFG, 4, 4
    CLR       r0, r0, 4         					// Clear SYSCFG[STANDBY_INIT] to enable OCP master port
    SBCO      r0, CONST_PRUCFG, 4, 4

	// prepare PRU shared memory access	
	MOV r0, 0x000000120
	MOV r1, CTPPR_0
	ST32 r0, r1
	
	MOV r0, 0x00100000
	MOV r1, CTPPR_1
	ST32 r0, r1
	
	// Setup IEP Timer
	LBCO    r6, CONST_IEP, 0x40, 40                 // Read all 10 32-bit CMP registers into r6-r15
    OR      r6, r6, 0x03                            // Set count reset and enable compare 0 event
	
	// Set loop period to 1 uS, or 1,000 nS
    MOV     r8,1000 								// 1 Us precision

    SBCO    r6, CONST_IEP, 0x40, 40                 // Save 10 32-bit CMP registers

    MOV     r2, 0x00000551                          // Enable counter, configured to count nS (increments by 5 each clock)
    SBCO    r2, CONST_IEP, 0x00, 4                  // Save IEP GLOBAL_CFG register

	// wait till a 1 is read
	WBS r31.t3
	
	// FOR DEBUG
	CLR r30.t5
	
	// r4 starts with 0, and incremented by 1 every IEP event
	MOV r4, 0	
	MOV r5, 0	
MAINLOOP:
	JMP WAIT
	
RET_MAIN:	
	// increment and store uint32_t into shared ram (4 bytes)
	ADD r4, r4, 1

	// Send Counter 32 bits R4
SEND_DATA:	
	SBCO r4, CONST_PRUSHAREDRAM, 0, 8
	
	//JMP MAINLOOP 	// FOR DEBUG
	QBBS  MAINLOOP, r31.t3
END_PROCESS:	
    // Send notification to Host for program completion
    MOV r31.b0, PRU0_ARM_INTERRUPT+16

    HALT
	
    // =============================================================	
WAIT:
    LBCO    r2, CONST_IEP, 0x44, 4      // Load CMP_STATUS register
    QBBC    WAIT, r2, 0                 // Wait until counter times out
    SBCO    r2, CONST_IEP, 0x44, 4      // Clear counter timeout bit
	
	// DEBUG PART
	QBBS    PIN_HIGH, r30.t5         	// Read Port Level
	//set pin to high
    SET r30.t5
	JMP 	end_WAIT
PIN_HIGH:	
	// clear pin to confirm read
    CLR r30.t5
end_WAIT:
    JMP RET_MAIN




    
