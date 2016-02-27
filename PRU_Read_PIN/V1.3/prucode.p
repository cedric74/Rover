// prucode.p, Version Cedric Toncanier
// Version V 1.3, 2015-04-07

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
	
	// Set Pin P9 27 to Low
	CLR r30.t5
	
	// Setup IEP Timer
	LBCO    r6, CONST_IEP, 0x40, 40                 // Read all 10 32-bit CMP registers into r6-r15
    OR      r6, r6, 0x03                            // Set count reset and enable compare 0 event
	
	// Set loop period to 1 uS, or 1,000 nS
    MOV     r8,1000 								// 1 Us precision

    SBCO    r6, CONST_IEP, 0x40, 40                 // Save 10 32-bit CMP registers

    MOV     r2, 0x00000551                          // Enable counter, configured to count nS (increments by 5 each clock)
    SBCO    r2, CONST_IEP, 0x00, 4                  // Save IEP GLOBAL_CFG register

	// ===================PULSE START SONAR PROCESS =========================
	SET r30.t5										// Set Pin P9 27 to High
	MOV r5, 5
WAIT_5_US:
	CALL WAIT										// Wait 1 us										
	SUB r5, r5, 1									// Index Loop
	QBNE WAIT_5_US, r5, 0							//Loop 5 time (us)
	
	CLR r30.t5										// Set Pin P9 27 to Low
	MOV r4, 0										// Clear Time us Counter
	
	// .... Maybe need to Add Some Delays for tje pin swith to low level
	
	WBS r31.t3										// wait till a 1 is read
	// =================== MAIN LOOP FUNCTION ===============================
MAIN_LOOP:
	CALL WAIT										// Wait 1 us
	ADD r4, r4, 1									// increment and store uint32_t into shared ram (4 bytes)

SEND_DATA:											// Send Counter 32 bits R4
	SBCO r4, CONST_PRUSHAREDRAM, 0, 8
	QBBS  MAIN_LOOP, r31.t3
END_PROCESS:	
    MOV r31.b0, PRU0_ARM_INTERRUPT+16				// Send notification to Host for program completion

    HALT											// End Program
	
    // =================== WAIT  FUNCTION ===============================	
WAIT:
    LBCO    r2, CONST_IEP, 0x44, 4      			// Load CMP_STATUS register
    QBBC    WAIT, r2, 0                 			// Wait until counter times out
    SBCO    r2, CONST_IEP, 0x44, 4      			// Clear counter timeout bit
	
	ret												// Return to Call Function
