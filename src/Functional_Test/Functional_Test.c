/*
 ============================================================================
 Name        : Functional_Test.c
 Author      : Cedric T.
 Version     : 1.0
 Date		 : 18/02/2015
 Description :
 ============================================================================
 */
 /*******************************************
*			  I N C L U D E 			   *
********************************************/
#include "Functional_Test.h"

/*******************************************
*               D E F I N E                *
********************************************/
// Time
#define		DELAYS_1_S					1000000	// in us,
#define		DELAYS_500_MS				500000	// in us,
#define		DELAYS_250_MS				250000	// in us,

/*******************************************
*   P R O T O T Y P E   F U N C T I O N S  *
********************************************/

/*******************************************
*	 G L O B A L   V A R I A B L E S  	   *
********************************************/


/*******************************************
*          F U N C T I O N S    		   *
********************************************/


/*
 ============================================
 Function     : Test_init()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void Test_init(){
	Lib_Servo_init();

	// Init Gpio Lib
	beh_BBB_gpio_init();

	// Init Sonar
	Lib_Sonar_Init();
}

/*
 ============================================
 Function     : test_PWM()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void test_Sonar(){
	// Sonar Scanning
	Lib_Sonar_Ping();
}

/*
 ============================================
 Function     : test_PWM()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void test_PWM(){

	servo_180();
}


void servo_180(){
	int iPos;

		for(iPos = LEFT_0 ; iPos <= RIGHT_0; iPos++){
//			if(u8StopThread == STOP_PROCESS){
//				// Stop Process
//				return STOP_PROCESS;
//			}
			// ************  POSITION SCANNING **************
			// Turn Sonar to Request Position
				Lib_Servo_Sonar_Control(iPos);

				// Maybe Need some Delay to be at the right place
				usleep(DELAYS_250_MS);
		}

		// Turn Sonar to Center Position
		Lib_Servo_Sonar_Control(CENTER);

		// Delay More Time To Change Pos from Right 0 To Center Pos
		usleep(DELAYS_500_MS);

//		return RUNNING_PROCESS;
}



