/**
 ********************************************************************************
 * @file    crc8.h
 * @author  Embedded Area
 * @date    Jan 26, 2021
 * @brief
 ********************************************************************************
 */

#ifndef INC_CRC8_H_
#define INC_CRC8_H_

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/
#include <stddef.h>
#include <stdint.h>

/************************************
 * MACROS AND DEFINES
 ************************************/

/************************************
 * TYPEDEFS
 ************************************/

/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/

/**crc8
 * @brief Calculates the crc of the message
 * @param message: address of message
 * @param len: length of the message
 * @return CRC value
 */
uint8_t crc8(const char *message, uint16_t len);


#ifdef __cplusplus
}
#endif

#endif
