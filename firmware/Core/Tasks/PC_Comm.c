/**
  ******************************************************************************
  * @file           PC_Comm.c
  * @brief          Contains tasks related with PC uart communication
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/**
* @brief Function implementing the pcRxManager thread.
* @param argument: Not used
* @retval None
*/
void PCRxManager(void const * argument)
{
  for(;;)
  {
    osDelay(1);
  }
}

/**
* @brief Function implementing the pcTxManager thread.
* @param argument: Not used
* @retval None
*/
void PCTxManager(void const * argument)
{
  for(;;)
  {
    osDelay(1);
  }
}

/**
* @brief Function implementing the pcCommManager thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_PcCommunicationManager */
void PcCommunicationManager(void const * argument)
{
  for(;;)
  {
    osDelay(1);
  }
}
