/**
 ******************************************************************************
 * @file           Sensors.c
 * @brief          Contains tasks related with sensor readings and usings
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

extern osThreadId gpsTaskHandle, mpu9265TaskHandle, batteryTaskHandle;

/**
 * @brief Function implementing the gpsTask thread.
 * @param argument: Not used
 * @retval None
 */
void GPSTask(void const * argument)
{
	osThreadTerminate(gpsTaskHandle);
	for(;;)
	{
		osDelay(1);
	}
}


/**
 * @brief Function implementing the mpu9265Task thread.
 * @param argument: Not used
 * @retval None
 */
void MPU9265Task(void const * argument)
{
	osThreadTerminate(mpu9265TaskHandle);
	for(;;)
	{
		osDelay(1);
	}
}


/**
 * @brief Function implementing the batteryTask thread.
 * @param argument: Not used
 * @retval None
 */
void BatteryTask(void const * argument)
{
	osThreadTerminate(batteryTaskHandle);
	for(;;)
	{
		osDelay(1);
	}
}
