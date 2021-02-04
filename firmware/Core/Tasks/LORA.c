/**
 ******************************************************************************
 * @file           LORA.c
 * @brief          Contains tasks related with LORA communication
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "crc8.h"
#include "string.h"

extern osThreadId loraRxManagerHandle, loraTxManagerHandle, loraCommManagerHandle;


extern osMessageQId lora_tx_qHandle;	/** Queue for TX synchronization */
extern osMessageQId lora_rx_qHandle;	/** Queue for RX synchronization */
extern osSemaphoreId lora_rx_smphrHandle;

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
	uart_data_t task_received;
	uint8_t received_command[30];

	/*---------------DATA FRAME FORMAT ---------------
	 * -----------------------------------------------
	 * | 3 BYTE PREAMBLE | 50 BYTE DATA | 1 BYTE CRC |
	 * -----------------------------------------------
	 * -----------------------------------------------*/
	uint8_t data_to_send[55];	//Maximum packet size for LORA is 58 bytes
	data_to_send[0] = 0x55;	//First preamble byte is trivial
	data_to_send[1] = 0x55;	//Second preamble byte is trivial
	uint8_t number_of_preambles = 2;
	uint32_t crc;

	for(;;)
	{
		xQueueReceive(lora_tx_qHandle, (void *) &task_received, osWaitForever);
		if(circular_read_from(&circular_buf_to_lora_outgoing, received_command, task_received.start_index, task_received.length) == CIRC_READ_SUCCESS) {
			//Taking CRC of payload (preamble is not included)
			crc = crc8((const char *)received_command, task_received.length);
			//Copying (task_received->length) bytes of data
			memcpy(data_to_send + number_of_preambles, received_command, task_received.length);
			//Copying 1 bytes of CRC
			memcpy(data_to_send + number_of_preambles + task_received.length, &crc, 1);

			HAL_UART_Transmit_IT(&HUART_LORA, (uint8_t *) data_to_send, number_of_preambles + task_received.length + 1);
		}
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
