/**
 ********************************************************************************
 * @file    circular_buffers.h.h
 * @author  Embedded Area
 * @date    2020
 * @brief	Contains circular buffer implementation and related functions
 ********************************************************************************
 */

#ifndef INC_CIRCULAR_BUFFERS_H_
#define INC_CIRCULAR_BUFFERS_H_

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/

/************************************
 * MACROS AND DEFINES
 ************************************/

/************************************
 * TYPEDEFS
 ************************************/

/**
 * Circular buffer result types
 */
typedef enum {
	CIRC_WRITE_SUCCESS = 1,
	CIRC_WRITE_ERR_NO_SPACE,
	CIRC_READ_SUCCESS,
	CIRC_READ_FAILED
} circular_status_t;


/**
 * Struct type for the circular buffers (will be used for LORA, PC and maybe GPS)
 */
typedef struct {
	uint32_t size;				/** message length */
	uint32_t head;				/** writing point */
	uint32_t tail;				/** reading point */
	uint32_t remaining_length;	/** remaining empty buffer size */
	uint8_t *data;				/** address of the array used for storing data*/
} circular_buffers_t;

/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
/**
 * @brief This function writes the given buffer to the circular buffer at a number of size
 * Writing starts from head
 * @param circular address of circular buffer
 * @param source_buffer address of source buffer where the data is copied
 * @param size length of data to be written in bytes
 * @retval 1 for successful writing
 */
circular_status_t circular_write(circular_buffers_t *circular, const uint8_t *source_buffer, uint32_t size);

/**
 * @brief This function reads the circular buffer and copies to the dest buffer at a number of size
 * Reading starts from tail
 * @param circular address of circular buffer
 * @param dest_buffer address of destination buffer where the data is copied
 * @param size length of data to be copied in bytes
 * @retval 1 for successful reading
 */
circular_status_t circular_read(circular_buffers_t *circular, uint8_t *dest_buffer, uint32_t size);

/**
 * @brief This function reads the circular buffer from the given index and copies to the dest buffer at a number of size
 * Reading starts from given index
 * @param circular address of circular buffer
 * @param dest_buffer address of destination buffer where the data is copied
 * @param starting_index index of destination buffer where the data copying starts
 * @param size length of data to be copied in bytes
 * @retval 1 for successful reading
 */
circular_status_t circular_read_from(circular_buffers_t *circular, uint8_t *dest_buffer, uint32_t starting_index, uint32_t size);


#ifdef __cplusplus
}
#endif

#endif
