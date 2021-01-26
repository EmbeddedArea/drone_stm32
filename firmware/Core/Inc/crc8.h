/*
 * crc8.h
 *
 *  Created on: 26 Oca 2021
 *      Author: Azad
 */

#ifndef INC_CRC8_H_
#define INC_CRC8_H_

#include <stddef.h>
#include <stdint.h>

/**crc8
 * @brief Calculates the crc of the message
 * @param message: address of message
 * @param len: length of the message
 * @return CRC value
 */
uint8_t crc8(const char *message, uint16_t len);

#endif /* INC_CRC8_H_ */
