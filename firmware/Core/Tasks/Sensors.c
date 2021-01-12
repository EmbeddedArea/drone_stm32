/**
 ******************************************************************************
 * @file           Sensors.c
 * @brief          Contains tasks related with sensor readings and usings
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "math.h"
#include "mpu9250.h"
#include "string.h"
#include "stdio.h"


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
	char text[30];
	const float alpha = 0.5;

	double fXg = 0;
	double fYg = 0;
	double fZg = 0;

	double pitch, roll, Xg, Yg, Zg;


	int ret = mpu_init();

	memset(text, 0, 50);
	sprintf((char *) text,"mpu_init:: %d\n", ret);
	HAL_UART_Transmit(&HUART_PC, (uint8_t *) text, strlen((const char *)text), 0xFF);

	int who = whoAmI();

	memset(text, 0, 50);
	sprintf((char *) text,"WhoAmI:: %d\n", who);
	HAL_UART_Transmit(&HUART_PC, (uint8_t *) text, strlen((const char *)text), 0xFF);

	who = whoAmIAK8963();

	memset(text, 0, 50);
	sprintf((char *) text,"whoAmIAK8963:: %d\n", who);
	HAL_UART_Transmit(&HUART_PC, (uint8_t *) text, strlen((const char *)text), 0xFF);


	for(;;)
	{
		readSensor();

		Xg = getAccelX_mss();
		Yg = getAccelY_mss();
		Zg = getAccelZ_mss();

		//Low Pass Filter
		fXg = Xg * alpha + (fXg * (1.0 - alpha));
		fYg = Yg * alpha + (fYg * (1.0 - alpha));
		fZg = Zg * alpha + (fZg * (1.0 - alpha));

		//Roll & Pitch Equations
		roll  = (atan2(-fYg, fZg)*180.0)/M_PI;
		pitch = (atan2(fXg, sqrt(fYg*fYg + fZg*fZg))*180.0)/M_PI;

		memset(text, 0, 50);
		sprintf((char *) text,"Roll:%05d:Pitch:%05d:\n", (int)(100.0*roll), (int)(100.0*pitch));
		HAL_UART_Transmit(&HUART_PC, (uint8_t *) text, strlen((const char *)text), 0xFF);
		osDelay(100);
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
