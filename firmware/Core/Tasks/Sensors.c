/**
 ******************************************************************************
 * @file           Sensors.c
 * @brief          Contains tasks related with sensor readings and usings
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "math.h"
#include "../MPU9250/mpu9250.h"
#include "../NEO_6M_GPS/gps.h"
#include "string.h"
#include "stdio.h"

extern DMA_HandleTypeDef hdma_i2c1_rx;
extern osSemaphoreId mpu9265_smphrHandle;
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
	osSemaphoreWait(mpu9265_smphrHandle, osWaitForever);
	__HAL_DMA_ENABLE_IT(&hdma_i2c1_rx, DMA_IT_TC);

	float pitch = 0.0f, roll = 0.0f;

#if SERIAL_DEBUG

	char text[65];

	int ret = MPU9250_Init();
	memset(text, 0, 65);
	sprintf((char *) text,"mpu_init: %d\n", ret);
	send_message_to_pc(text);

	int who = MPU9250_WhoAmI();

	memset(text, 0, 65);
	sprintf((char *) text,"WhoAmI: %d\n", who);
	send_message_to_pc(text);

	who = MPU9250_WhoAmIAK8963();

	memset(text, 0, 65);
	sprintf((char *) text,"whoAmIAK8963: %d\n", who);
	send_message_to_pc(text);

#else
	MPU9250_Init();
#endif

	uint32_t time, deltaT;
	uint32_t lastTime = 0;

	for(;;)
	{
		MPU9250_ReadSensor();
		time = HAL_GetTick();
		deltaT = time - lastTime;
		lastTime = time;
		ComplementaryFilter(&pitch, &roll, deltaT);
#if SERIAL_DEBUG
		memset(text, 0, 65);
		sprintf((char *) text,"Roll:%05d:Pitch:%05d:\r\n", (int)(100.0*roll), (int)(100.0*pitch));
		send_message_to_pc(text);
#endif

		osDelay(80);

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
