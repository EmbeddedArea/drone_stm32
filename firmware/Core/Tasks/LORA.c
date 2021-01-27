/**
 ******************************************************************************
 * @file           LORA.c
 * @brief          Contains tasks related with LORA communication
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

extern osThreadId loraRxManagerHandle, loraTxManagerHandle, loraCommManagerHandle;

/**
 * @brief Function implementing the loraRxManager thread.
 * @param argument: Not used
 * @retval None
 */
void LORARxManager(void const * argument)
{
	osThreadTerminate(loraRxManagerHandle);
	for(;;)
	{
		osDelay(1);
	}
}

/**
 * @brief Function implementing the loraTxManager thread.
 * @param argument: Not used
 * @retval None
 */
void LORATxManager(void const * argument)
{
	osThreadTerminate(loraTxManagerHandle);
	for(;;)
	{
		osDelay(1);
	}
}

/**
 * @brief Function implementing the loraCommManager thread.
 * @param argument: Not used
 * @retval None
 */
void LORACommunicationManager(void const * argument)
{
	osThreadTerminate(loraCommManagerHandle);
	for(;;)
	{
		osDelay(1);
	}
}
