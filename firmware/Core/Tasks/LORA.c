/**
  ******************************************************************************
  * @file           LORA.c
  * @brief          Contains tasks related with LORA communication
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/**
* @brief Function implementing the loraRxManager thread.
* @param argument: Not used
* @retval None
*/
void LORARxManager(void const * argument)
{
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
  for(;;)
  {
    osDelay(1);
  }
}
