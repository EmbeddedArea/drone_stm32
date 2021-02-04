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

uint8_t lora_rx_dma_buffer[LORA_RX_DMA_BUFFER_LEN]; 			/** \brief An array for storing data coming from Lora DMA */

uint8_t lora_incoming_circular[LORA_CIRCULAR_BUFFER_SIZE]; 	/** \brief Static array for storing data coming from Lora */
uint8_t lora_outgoing_circular[LORA_CIRCULAR_BUFFER_SIZE]; 	/** \brief Static array for writing data to Lora */

/**
 * circular_buf_lora_incoming
 */
circular_buffers_t circular_buf_lora_incoming = {
		.size = LORA_CIRCULAR_BUFFER_SIZE,
		.head = 0,
		.tail = 0,
		.remaining_length = LORA_CIRCULAR_BUFFER_SIZE,
		.data = lora_incoming_circular
};

/**
 * circular_buf_to_lora_outgoing
 */
circular_buffers_t circular_buf_to_lora_outgoing = {
		.size = LORA_CIRCULAR_BUFFER_SIZE,
		.head = 0,
		.tail = 0,
		.remaining_length = LORA_CIRCULAR_BUFFER_SIZE,
		.data = lora_outgoing_circular
};


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
