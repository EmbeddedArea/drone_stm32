/**
 ******************************************************************************
 * @file           circular_buffers.c
 * @brief          Contains circular buffer implementation, related functions
 * and global variables.
 * @author 		Azad Karataş
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "string.h"
#include "circular_buffers.h"

/**
 * @brief This function writes the given buffer to the circular buffer at a number of size
 * Writing starts from head
 * @param circular: address of circular buffer
 * @param source_buffer: address of source buffer where the data is copied
 * @param size: length of data to be written in bytes
 * @retval 1 for successful writing
 */
circular_status_t circular_write(circular_buffers_t *circular, const uint8_t *source_buffer, uint32_t size){
	/* Check if the circular buffer is empty enough */
	if(circular->remaining_length >= size){
		do {
			circular->data[circular->head] = *source_buffer++;
			circular->head = (circular->head + 1) % circular->size;
			circular->remaining_length--;
		} while(--size != 0);
		return CIRC_WRITE_SUCCESS;
	}
	else {
		return CIRC_WRITE_ERR_NO_SPACE;
	}
}


/**
 * @brief This function reads the circular buffer and copies to the dest buffer at a number of size
 * Reading starts from tail
 * @param circular: address of circular buffer
 * @param dest_buffer: address of destination buffer where the data is copied
 * @param size: length of data to be copied in bytes
 * @retval 1 for successful reading
 */
circular_status_t circular_read(circular_buffers_t *circular, uint8_t *dest_buffer, uint32_t size){
	do {
		*dest_buffer++ = circular->data[circular->tail];
		circular->tail = (circular->tail + 1) % circular->size;
		circular->remaining_length++;
	} while(--size != 0);
	return CIRC_READ_SUCCESS;
}

/**
 * @brief This function reads the circular buffer from the given index and copies to the dest buffer at a number of size
 * Reading starts from given index
 * @param circular: address of circular buffer
 * @param dest_buffer: address of destination buffer where the data is copied
 * @param starting_index: index of destination buffer where the data copying starts
 * @param size: length of data to be copied in bytes
 * @retval 1 for successful reading
 */
circular_status_t circular_read_from(circular_buffers_t *circular, uint8_t *dest_buffer, uint32_t starting_index, uint32_t size){
	do {
		*dest_buffer++ = circular->data[starting_index];
		starting_index = (starting_index + 1) % circular->size;
		circular->remaining_length++;
	} while(--size != 0);
	return CIRC_READ_SUCCESS;
}


