/**
 ******************************************************************************
 * @file           Controls.c
 * @brief          Contains tasks related with LORA communication
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

extern osThreadId periodicTaskHandle, motorControlTasHandle;

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
