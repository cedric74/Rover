/*
 * Lib_Algo.c
 *
 *  Created on: Apr 09, 2015
 *      Author: cedric
 */

/*******************************************
*			  I N C L U D E 			   *
********************************************/
#include <time.h>

#include "Lib_Algo.h"

/*******************************************
*               D E F I N E                *
********************************************/
// Dim Rover :
// Width : 22.5 cm	
// Length: 24.5 cm
#define 	WIDTH_ROVER					22.5	// For Security Calculate distance with 24 cm
#define 	LENGTH_ROVER				24.5

// Trigonometry
// Cos A = Adjacent / Hypo -> Adjacent = Width /2  -> Distance mini X  for 22 cm (10 cm of Security)

// For 22.5' =  22.98 cm
// For 45'   =  26.70 cm
// For 67.5' =  41.35 cm

// Distance
#define 	MINI_DIST_CENTER			(LENGTH_ROVER + 10)		// in CM   (10 cm of Security)
#define		MINI_DIST_67_5				41.35					// in CM
#define		MINI_DIST_45				26.70					// in CM
#define		MINI_DIST_22_5				22.98					// in CM

// Time
#define		DELAYS_1_S					1000000	// in us,
#define		DELAYS_500_MS				500000	// in us,
#define		DELAYS_250_MS				250000	// in us,

// For Debug
//#define		NO_SONAR				0

// Time Tick Algo
#define	TICK_LOOP_10_MS					10000	// Too Short for Sonar
#define	TICK_LOOP_100_MS				100000	// Maybe To Short
#define	TICK_LOOP_200_MS				200000

// State Thread
#define	RUNNING_PROCESS		1
#define	STOP_PROCESS		0

/*******************************************
*   T Y P E D E F   &  C O N S T A N T E   *
********************************************/

/*******************************************
*	 G L O B A L   V A R I A B L E S  	   *
********************************************/
// Variables Thread Algo
pthread_t 		thread_Algo;

static float	tabAreaScannig[NBE_SONAR_ROTATE]={0};

// Variables to Avoid Infinite Loop
static			eCtrlDirection	iPreviousCmd =  STOP_MOVE;

// Variable Flag Error Mode

// Variable Stop Thread Algo
unsigned char u8StopThread;

/*******************************************
*	        F U N C T I O N S   	       *
********************************************/
void 			Lib_Algo_Test();
static void * 	Lib_Algo_Thread(void * p);


void 			Lib_Algo_Ball_Rover(void);
static void  	Lib_Algo_Roaming_Rover(void);
static int 	    Lib_Algo_All_Area_Scanning();
static void 	Lib_Algo_Scanning(eServo_Sonar_Rotate valueRotate);


/*
 ============================================
 Function     : Lib_Algo_Ball_Rover()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void Lib_Algo_Ball_Rover(void){


	// Declarations Variables
	int iRet;

	//tabAreaScannig[CENTER]  > MINI_DIST_CENTER

	// Go Head
	//Lib_motor_control(FORWARD);
	//printf(" -> FORWARD \n");
	// Scanning Area At CenterPosition
	//Lib_Algo_Scanning(CENTER);
	// Prevent Infinite loop
	//iPreviousCmd = FORWARD;

	printf( "Position : \n" );

	iRet = Lib_Algo_All_Area_Scanning();

	int i = 0 ;
	for(i = LEFT_0 ; i <= RIGHT_0; i++){
		// ************  POSITION SCANNING **************
		// tabAreaScannig[CENTER]  > MINI_DIST_CENTER
		printf("pos : %d  , Distance : 4.2%F\n", LEFT_0, tabAreaScannig[i] );

	}
}


/*
 ============================================
 Function     : Algo_start()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void Lib_Algo_Start(){

	// Process Running
	u8StopThread = RUNNING_PROCESS;

	// Thread Execute Algo
	pthread_create (&thread_Algo, NULL, &Lib_Algo_Thread, NULL);
}

/*
 ============================================
 Function     : Lib_Algo_Thread()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void * Lib_Algo_Thread(void * p){
	do{
		Lib_Algo_Roaming_Rover();

		// Sleep
		usleep( TICK_LOOP_10_MS);
	}while(u8StopThread == RUNNING_PROCESS);

	return NULL;
}

/*
 ============================================
 Function     : Lib_Algo_Test()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void Lib_Algo_Test(){
	// DEBUG SERVO AND SONAR
	Lib_Algo_All_Area_Scanning();
	sleep(2);

	// DEBUG MOTOR
	// Start Go Head
	Lib_motor_control(FORWARD);
	printf(" -> FORWARD \n");
	sleep(2);

	Lib_motor_control(STOP_MOVE);
	printf(" -> STOP \n");
	sleep(1);

	// Rotate 90 to The Right
	Lib_motor_rover_Rotate(ROTATE_RIGHT, TIME_ROTATE_90);
	printf(" -> Rotate 90 to The Right \n");
	sleep(2);

	// U TURN
	Lib_motor_rover_U_Turn();
	printf(" -> U_Turn \n");
}

/*
 ============================================
 Function     : Lib_Algo_Roaming_Rover()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void Lib_Algo_Roaming_Rover(void){

	// Declarations Variables
	int iRet;

	// Mode Error

	// ********* CHECK FRONT OF IF IT'S CLEAR	********************* // Maybe need to check for Angle 45 and 22.5 Left and Right
	if((tabAreaScannig[CENTER]  > MINI_DIST_CENTER)&& (tabAreaScannig[RIGHT_67_5]  > MINI_DIST_67_5) && (tabAreaScannig[LEFT_67_5]  > MINI_DIST_67_5) )
	{
		// Go Head
		Lib_motor_control(FORWARD);
		printf(" -> FORWARD \n");
		// Scanning Area At CenterPosition
		Lib_Algo_Scanning(CENTER);

		// Prevent Infinite loop
		iPreviousCmd = FORWARD;
		
		// End Function
		return;

	}else{
		// Stop Move
		Lib_motor_control(STOP_MOVE);
		printf(" -> STOP_MOVE \n");

		// Scan All Area from current position
		iRet = Lib_Algo_All_Area_Scanning();
		if(iRet == STOP_PROCESS){
			return;
		}


		//iPreviousCmd = STOP_MOVE;
	}

	// ********* CHECK RIGHT SIDE IF IT'S CLEAR	*********************
	if( (tabAreaScannig[RIGHT_0]  > MINI_DIST_CENTER) && (tabAreaScannig[RIGHT_22_5]  > MINI_DIST_CENTER) ){
		//if(tabAreaScannig[RIGHT_45]  > MINI_DIST_CENTER){
			//if(tabAreaScannig[RIGHT_67_5]  > MINI_DIST_CENTER){
				if(iPreviousCmd != ROTATE_LEFT){								
					printf(" -> Rotate 90 to The Right \n");
					// Rotate 90 to The Right
					Lib_motor_rover_Rotate(ROTATE_RIGHT, TIME_ROTATE_90);

					// Scan All Area from new position
					iRet = Lib_Algo_All_Area_Scanning();
					if(iRet == STOP_PROCESS){
						return;
					}

					// Prevent Infinite loop
					iPreviousCmd = ROTATE_RIGHT;
				}
			//}
		//}
	}else{
		// ********* CHECK LEFT SIDE IF IT'S CLEAR	*********************
		if((tabAreaScannig[LEFT_0]  > MINI_DIST_CENTER) && (tabAreaScannig[LEFT_22_5]  > MINI_DIST_CENTER) ) {
			//if(tabAreaScannig[LEFT_45]  > MINI_DIST_CENTER){
				//if(tabAreaScannig[LEFT_67_5]  > MINI_DIST_CENTER){
					if(iPreviousCmd != ROTATE_RIGHT){								
						printf(" -> Rotate 90 to The Left \n");
						// Rotate 90 to The Left
						Lib_motor_rover_Rotate(ROTATE_LEFT, TIME_ROTATE_90);
						// Scan All Area from new position
						iRet = Lib_Algo_All_Area_Scanning();
						if(iRet == STOP_PROCESS){
							return;
						}

						// Prevent Infinite loop
						iPreviousCmd = ROTATE_LEFT;
					}
				//}
			//}
		}else{
			// ********* NOT SIDE CLEAR, NEED TO DO U TURN	*********************
			printf(" -> U TURN \n");

			// Rotate 180
			Lib_motor_rover_U_Turn();

			// Scan All Are from new position
			iRet = Lib_Algo_All_Area_Scanning();
			if(iRet == STOP_PROCESS){
				return;
			}
		}
	}
}

/*
 ============================================
 Function     : Lib_Algo_All_Area_Scanning()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
static int Lib_Algo_All_Area_Scanning(){

	int iPos;

	for(iPos = LEFT_0 ; iPos <= RIGHT_0; iPos++){
		if(u8StopThread == STOP_PROCESS){
			// Stop Process
			return STOP_PROCESS;
		}
		// ************  POSITION SCANNING **************
		Lib_Algo_Scanning(iPos);
	}

	// Turn Sonar to Center Position
	Lib_Servo_Sonar_Control(CENTER);

	// Delay More Time To Change Pos from Right 0 To Center Pos
	usleep(DELAYS_500_MS);

	return RUNNING_PROCESS;
}

/*
 ============================================
 Function     : Lib_Algo_Scanning()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
static void Lib_Algo_Scanning(eServo_Sonar_Rotate valueRotate){

	// Turn Sonar to Request Position
	Lib_Servo_Sonar_Control(valueRotate);

	// Maybe Need some Delay to be at the right place
	usleep(DELAYS_250_MS);

	#ifdef 	NO_SONAR
		// Test Debug Without Sonar
		// Generate A random Value for fDistance variables
		fDistance = rand() % 300;

		// Print Value
		printf(" fDistance : %4.2f , Random Generator \n" , fDistance);

	#else
		// Sonar Scanning
		Lib_Sonar_Ping();
	#endif



	// Save Value into the Table Area
	tabAreaScannig[valueRotate] = fDistance;

}

/*
 ============================================
 Function     : Lib_Algo_Stop()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void Lib_Algo_Stop(){

	int old_cancel_state;

	// Print End Process
	printf(" Stop Program \n");

	// Start Section critic
	pthread_setcancelstate (PTHREAD_CANCEL_DISABLE, &old_cancel_state);

	// Set State Process to Stop
	u8StopThread = STOP_PROCESS;

	// End Section critic
	pthread_setcancelstate (old_cancel_state, NULL);

	// Wait for The End of the Thread
	pthread_join(thread_Algo, NULL);

	// Stop Motor
	Lib_motor_control(STOP_MOVE);
	printf(" -> STOP_MOVE \n");

	// Servo Center Position
	Lib_Servo_Sonar_Control(CENTER);

}

/*
 ============================================
 Function     : Lib_Algo_Init()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void Lib_Algo_Init(){

	// Init Gpio Lib
	beh_BBB_gpio_init();

	// Motor
	Lib_motor_init();
	CmdDirection = NO_CMD;

	// Init Servo Motor
	Lib_Servo_init();


	#ifdef 	NO_SONAR
		// Init Randon Generator
		srand(time(NULL));

	#else
		// Init Sonar
		Lib_Sonar_Init();
	#endif

	// Scanning the All Area at the Init Position
	Lib_Algo_All_Area_Scanning();
}
