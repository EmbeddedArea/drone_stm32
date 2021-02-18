/**
 ********************************************************************************
 * @file    Controls.c
 * @author  Embedded Area
 * @date    2021
 * @brief   Contains tasks related with LORA communication
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "main.h"
#include "cmsis_os.h"

/************************************
 * EXTERN VARIABLES
 ************************************/
extern osThreadId periodicTaskHandle, motorControlTasHandle;

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/

/************************************
 * PRIVATE TYPEDEFS
 ************************************/

/************************************
 * STATIC VARIABLES
 ************************************/

/************************************
 * GLOBAL VARIABLES
 ************************************/

/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/

/************************************
 * STATIC FUNCTIONS
 ************************************/

/************************************
 * GLOBAL FUNCTIONS
 ************************************/

/**
 * @brief Function implementing the periodicTask thread.
 * @param argument: Not used
 * @retval None
 */
void PeriodicTask(void const * argument)
{
	osThreadTerminate(periodicTaskHandle);
	for(;;)
	{
		osDelay(1);
	}
}


/**
 * @brief Function implementing the motorControlTas thread.
 * @param argument: Not used
 * @retval None
 */
void MotorControlTask(void const * argument)
{
	osThreadTerminate(motorControlTasHandle);
	for(;;)
	{
		osDelay(1);
	}
}
