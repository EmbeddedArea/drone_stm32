/**
 ********************************************************************************
 * @file    PC_Comm.c
 * @author  Embedded Area
 * @date    2021
 * @brief   Contains tasks related with PC uart communication
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "main.h"
#include "cmsis_os.h"
#include "string.h"
#include "stdio.h"
#include "circular_buffers.h"

/************************************
 * EXTERN VARIABLES
 ************************************/
extern osMessageQId pc_tx_qHandle;	/** Queue for TX synchronization */
extern osMessageQId pc_rx_qHandle;	/** Queue for RX synchronization */
extern osSemaphoreId pc_rx_smphrHandle;

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/

/************************************
 * PRIVATE TYPEDEFS
 ************************************/

/************************************
 * STATIC VARIABLES
 ************************************/
static uint8_t pc_rx_dma_buffer[PC_RX_DMA_BUFFER_LEN]; 			/** \brief An array for storing data coming from Uart DMA */
static uint8_t uart_pc_circular[PC_CIRCULAR_UART_BUFFER_SIZE]; 	/** \brief Static array for storing data coming from uart */
static uint8_t uart_to_pc_circular[PC_CIRCULAR_UART_BUFFER_SIZE]; 	/** \brief Static array for writing data to uart */

/**
 * circular_buf_pc
 */
static circular_buffers_t circular_buf_pc = {
		.size = PC_CIRCULAR_UART_BUFFER_SIZE,
		.head = 0,
		.tail = 0,
		.remaining_length = PC_CIRCULAR_UART_BUFFER_SIZE,
		.data = uart_pc_circular
};

/**
 * circular_buf_to_pc
 */
static circular_buffers_t circular_buf_to_pc = {
		.size = PC_CIRCULAR_UART_BUFFER_SIZE,
		.head = 0,
		.tail = 0,
		.remaining_length = PC_CIRCULAR_UART_BUFFER_SIZE,
		.data = uart_to_pc_circular
};

/************************************
 * GLOBAL VARIABLES
 ************************************/

/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/

/************************************
 * STATIC FUNCTIONS
 ************************************/

/************************************
 * GLOBAL FUNCTIONS
 ************************************/

/**
 * @brief This function handles the parsing and transmitting processes.
 * @param format: message format to be sent
 * @retval None
 */
void send_message_to_pc(const char * buffer )
{
	uart_data_t struct_for_queue;
	//Fill inside the received_frame struct
	struct_for_queue.start_index = circular_buf_to_pc.head;
	struct_for_queue.length = strlen(buffer);
	if(circular_write(&circular_buf_to_pc, (const uint8_t *) buffer, strlen(buffer)) == CIRC_WRITE_SUCCESS){
		if(xQueueSend(pc_tx_qHandle, (void *) &struct_for_queue, (TickType_t) 10) != pdPASS){	//Try sending to queue
			// If sending queue is failed, then remove the written buffer from circular buffer
			circular_buf_to_pc.remaining_length += struct_for_queue.length;
			circular_buf_to_pc.head = struct_for_queue.start_index;
#if SERIAL_DEBUG
			uint8_t text[50];
			// Failed to post the message, even after 10 ticks.
			memset(text, 0, 50);
			sprintf((char *) text, "Too busy... Failed to send to pc_tx_qHandle\n");
			HAL_UART_Transmit(&HUART_PC, (uint8_t *) text, strlen((const char *)text), 0xFF);
#endif
		}
		else {	//Sending queue is successful
#if 0
			uint8_t text[50];
			memset(text, 0, 50);
			sprintf((char *) text, "Queue to PC sent start_index:%d length:%d\n", (int) struct_for_queue.start_index, (int) struct_for_queue.length);
			HAL_UART_Transmit(&HUART_PC, (uint8_t *) text, strlen((const char *)text), 0xFF);
#endif
		}
	}
}


/**
 * @brief This task is used for frame decoding of received UART data.
 * It copies the DMA RX buffer to the circular buffer.
 * @param argument: Not used
 * @retval None
 */
void PCRxManager(void const * argument)
{
	osSemaphoreWait(pc_rx_smphrHandle, osWaitForever);	/* Starts with obtaining the free semaphore at initial */
	uint32_t rx_buffer_old_index = 0;
	uint32_t rx_buffer_new_index = 0;
	uart_data_t struct_for_queue;

	//struct_for_queue.circular_buffer_addr = &circular_buf_pc;	/* This address is fixed for all queue elements*/

	__HAL_UART_ENABLE_IT(&HUART_PC, UART_IT_IDLE);
	HAL_UART_Receive_DMA(&HUART_PC, pc_rx_dma_buffer, PC_RX_DMA_BUFFER_LEN);

#if SERIAL_DEBUG
	uint8_t text[100];
#endif
	for(;;)
	{
		osSemaphoreWait(pc_rx_smphrHandle, osWaitForever);
		rx_buffer_new_index = PC_RX_DMA_BUFFER_LEN - __HAL_DMA_GET_COUNTER(&HDMA_UART_PC);
		if(rx_buffer_new_index != rx_buffer_old_index) {	/* Simple check in receiving data */
			if (rx_buffer_new_index > rx_buffer_old_index) { /* Normal FIFO mode */
				//Fill inside the received_frame struct
				struct_for_queue.start_index = circular_buf_pc.head;
				struct_for_queue.length = rx_buffer_new_index - rx_buffer_old_index;
				if(circular_write(&circular_buf_pc, (pc_rx_dma_buffer + rx_buffer_old_index), rx_buffer_new_index - rx_buffer_old_index) == CIRC_WRITE_SUCCESS){
					if(xQueueSend(pc_rx_qHandle, (void *) &struct_for_queue, (TickType_t) 10) != pdPASS){	//Try sending to queue
						// If sending queue is failed, then remove the written buffer from circular buffer
						circular_buf_pc.remaining_length += struct_for_queue.length;
						circular_buf_pc.head = struct_for_queue.start_index;
#if SERIAL_DEBUG
						// Failed to post the message, even after 10 ticks.
						memset(text, 0, 100);
						sprintf((char *) text, "Too busy... Failed to send to pc_rx_qHandle\n");
						HAL_UART_Transmit(&HUART_PC, (uint8_t *) text, strlen((const char *)text), 0xFF);
#endif
					}
					else {	//Sending queue is successful
#if SERIAL_DEBUG
						memset(text, 0, 100);
						sprintf((char *) text, "Queue sent start_index:%d length:%d\n", (int) struct_for_queue.start_index, (int) struct_for_queue.length);
						HAL_UART_Transmit(&HUART_PC, (uint8_t *) text, strlen((const char *)text), 0xFF);
#endif
					}
				}
				else {	// Not enough length in circular buffer for new commands
#if SERIAL_DEBUG
					memset(text, 0, 100);
					sprintf((char *) text, "Not enough length in circular buffer for new commands\n");
					HAL_UART_Transmit(&HUART_PC, (uint8_t *) text, strlen((const char *)text), 0xFF);
#endif
				}
			}
			else {	/* Overflow FIFO mode */
				if(circular_buf_pc.remaining_length >= (PC_RX_DMA_BUFFER_LEN - rx_buffer_old_index + rx_buffer_new_index)) {	/* Check if the circular buffer is empty enough */
					//Fill inside the received_frame struct
					struct_for_queue.start_index = circular_buf_pc.head;
					struct_for_queue.length = PC_RX_DMA_BUFFER_LEN - rx_buffer_old_index + rx_buffer_new_index;
					/* First process the data end of the buffer */
					circular_write(&circular_buf_pc, (pc_rx_dma_buffer + rx_buffer_old_index), PC_RX_DMA_BUFFER_LEN - rx_buffer_old_index);
					if (rx_buffer_new_index > 0) {/* If remains, process the data beginning of the buffer */
						circular_write(&circular_buf_pc, pc_rx_dma_buffer, rx_buffer_new_index);
					}

					if(xQueueSend(pc_rx_qHandle, (void *) &struct_for_queue, (TickType_t) 10) != pdPASS){	//Try sending to queue
#if SERIAL_DEBUG
						// Failed to post the message, even after 10 ticks.
						memset(text, 0, 100);
						sprintf((char *) text, "Too busy... Failed to send to lora_rx_qHandle\n");
						HAL_UART_Transmit(&HUART_PC, (uint8_t *) text, strlen((const char *)text), 0xFF);
#endif
					}
				}
				else {// Not enough length in circular buffer for new commands
#if SERIAL_DEBUG
					memset(text, 0, 100);
					sprintf((char *) text, "Not enough length in circular buffer for new commands\n");
					HAL_UART_Transmit(&HUART_PC, (uint8_t *) text, strlen((const char *)text), 0xFF);
#endif
				}
			}
		}

		rx_buffer_old_index = rx_buffer_new_index;
		// If DMA counter is at the end of the buffer, set it to 0
		if (rx_buffer_old_index == PC_RX_DMA_BUFFER_LEN) {
			rx_buffer_old_index = 0;
		}
	}
}

/**
 * @brief Function implementing the pcTxManager thread.
 * @param argument: Not used
 * @retval None
 */
void PCTxManager(void const * argument)
{
	uart_data_t task_received;
	uint8_t received_command[30];
#if SERIAL_DEBUG
	uint8_t text[100];
#endif
	for(;;)
	{
		xQueueReceive(pc_tx_qHandle, (void *) &task_received, osWaitForever);
		if(circular_read_from(&circular_buf_to_pc, received_command, task_received.start_index, task_received.length) == CIRC_READ_SUCCESS) {
			HAL_UART_Transmit(&HUART_PC, (uint8_t*)received_command, task_received.length, 0xFF);
		}
	}
}

/**
 * @brief This task parses the incoming commands (pc_rx_qHandle) and send them to related functions.
 * @param argument: Not used
 * @retval None
 */
void PcCommunicationManager(void const * argument)
{
	uint8_t received_command[30];
	uart_data_t msg_received;

	for(;;)
	{
		xQueueReceive(pc_rx_qHandle, (void *) &msg_received, osWaitForever);
		if(circular_read_from(&circular_buf_pc, received_command, msg_received.start_index, msg_received.length) == CIRC_READ_SUCCESS) {
			//Incoming commands are being examined
			switch(received_command[0]){
			case 'a': {

				send_message_to_pc("a case");
				break;
			}
			case 'b': {

				send_message_to_pc("b case");
				break;
			}
			default:{	// Unknown command
				send_message_to_pc("default case");
				break;
			}
			}

		}
		else {
#if SERIAL_DEBUG
			//todo: Some log function is needed!
			HAL_UART_Transmit(&HUART_PC, (uint8_t *) "CIRC_READ_FAILED\n", 17, 0xFF);
#endif
		}
	}
}
