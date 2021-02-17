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
#include "drone_infos.h"

extern osThreadId loraRxManagerHandle, loraTxManagerHandle, loraCommManagerHandle;
extern osMessageQId lora_tx_qHandle;	/** Queue for TX synchronization */
extern osMessageQId lora_rx_qHandle;	/** Queue for RX synchronization */
extern osSemaphoreId lora_rx_smphrHandle, lora_tx_smphrHandle;

uint8_t lora_rx_dma_buffer[LORA_RX_DMA_BUFFER_LEN]; 			/** \brief An array for storing data coming from Lora DMA */

uint8_t lora_incoming_circular[LORA_CIRCULAR_BUFFER_SIZE]; 	/** \brief Static array for storing data coming from Lora */
uint8_t lora_outgoing_circular[LORA_CIRCULAR_BUFFER_SIZE]; 	/** \brief Static array for writing data to Lora */

/**
 * circular_buf_lora_incoming
 */
circular_buffers_t circular_buf_from_lora = {
		.size = LORA_CIRCULAR_BUFFER_SIZE,
		.head = 0,
		.tail = 0,
		.remaining_length = LORA_CIRCULAR_BUFFER_SIZE,
		.data = lora_incoming_circular
};

/**
 * circular_buf_to_lora_outgoing
 */
circular_buffers_t circular_buf_to_lora = {
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
	osSemaphoreWait(lora_rx_smphrHandle, osWaitForever);	/* Starts with obtaining the free semaphore at initial */
	uint32_t rx_buffer_old_index = 0;
	uint32_t rx_buffer_new_index = 0;
	uart_data_t struct_for_queue;
#if SERIAL_DEBUG
	char text[60];
#endif

	__HAL_UART_ENABLE_IT(&HUART_LORA, UART_IT_IDLE);
	HAL_UART_Receive_DMA(&HUART_LORA, lora_rx_dma_buffer, LORA_RX_DMA_BUFFER_LEN);

	/* Infinite loop */
	for(;;)
	{
		osSemaphoreWait(lora_rx_smphrHandle, osWaitForever);
		send_message_with_lora("lora received\n");
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
	uint8_t received_command[50];

	/*----------------- DATA FRAME FORMAT ------------
	 * -----------------------------------------------
	 * | 2 BYTE PREAMBLE | 50 BYTE DATA | 1 BYTE CRC |
	 * -----------------------------------------------
	 * -----------------------------------------------*/
	uint8_t data_to_send[53];	//Maximum packet size for LORA is 53 bytes
	data_to_send[0] = 0x55;		//First preamble byte is trivial
	data_to_send[1] = 0x55;		//Second preamble byte is trivial
	uint8_t number_of_preambles = 2;
	uint32_t crc;

	for(;;)
	{
		xQueueReceive(lora_tx_qHandle, (void *) &task_received, osWaitForever);
		if(circular_read_from(&circular_buf_to_lora, received_command, task_received.start_index, task_received.length) == CIRC_READ_SUCCESS) {
			//Taking CRC of payload (preamble is not included)
			crc = crc8((const char *)received_command, task_received.length);

			//Copying (task_received->length) bytes of data
			memcpy(data_to_send + number_of_preambles, received_command, task_received.length);

			//Copying 1 bytes of CRC
			memcpy(data_to_send + number_of_preambles + task_received.length, &crc, 1);

			HAL_UART_Transmit_IT(&HUART_LORA, (uint8_t *) data_to_send, number_of_preambles + task_received.length + 1);

			osDelay(4);	// Max delay for 53 byte transmission at a rate of 115200bps
		}
	}
}

/**
 * @brief Parses and processes the incoming data from Lora
 * @param argument: Not used
 * @retval None
 */
void LORACommunicationManager(void const * argument)
{
	/* USER CODE BEGIN LORACommunicationManager */
	/* Infinite loop */
	for(;;)
	{
		osDelay(1);
	}
	/* USER CODE END LORACommunicationManager */
}


/**
 * @brief Send a message via LORA to ground station with
 * this function. It handles the parsing and transmitting processes.
 * @param format: message format to be sent
 * @retval None
 */
void send_message_with_lora(const char * buffer )
{
	uart_data_t struct_for_queue;
	//Fill inside the received_frame struct
	struct_for_queue.start_index = circular_buf_to_lora.head;
	struct_for_queue.length = strlen(buffer);
	if(circular_write(&circular_buf_to_lora, (const uint8_t *) buffer, strlen(buffer)) == CIRC_WRITE_SUCCESS){
		if(xQueueSend(lora_tx_qHandle, (void *) &struct_for_queue, (TickType_t) 10) != pdPASS){	//Try sending to queue
			// If sending queue is failed, then remove the written buffer from circular buffer
			circular_buf_to_lora.remaining_length += struct_for_queue.length;
			circular_buf_to_lora.head = struct_for_queue.start_index;
#if SERIAL_DEBUG
			uint8_t text[50];
			// Failed to post the message, even after 10 ticks.
			memset(text, 0, 50);
			sprintf((char *) text, "Too busy... Failed to send to lora_tx_qHandle\n");
			HAL_UART_Transmit(&HUART_PC, (uint8_t *) text, strlen((const char *)text), 0xFF);
#endif
		}
		else {	//Sending queue is successful
#if 0
			uint8_t text[50];
			memset(text, 0, 50);
			sprintf((char *) text, "Queue to LORA sent start_index:%d length:%d\n", (int) struct_for_queue.start_index, (int) struct_for_queue.length);
			HAL_UART_Transmit(&HUART_PC, (uint8_t *) text, strlen((const char *)text), 0xFF);
#endif
		}
	}
}
