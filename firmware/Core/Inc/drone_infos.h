/*
 * drone_infos.h
 *
 *  Created on: 23 Eki 2020
 *      Author: Azad
 */

#ifndef INC_DRONE_INFOS_H_
#define INC_DRONE_INFOS_H_

#include "stdint.h"

/*
 * Message Format and Indices
 * |address|type|control=CTRL_MOTOR_PWM_VALUES|motorSolAnalog|motorSa�AnalogDikey|motorSa�AnalogYatay|crc(2 byte)
 * |address|type|control=CTRL_SPECIAL_MOTOR_MOVEMENT|TURN_RIGHT|crc(2 byte)
 * |address|type|control=CTRL_OTHER_MESSAGES|OTONOM|crc(2 byte)
 * 0x49 0x0A 0x00
 */
#define LORA_RX_MSG_ADDRESS_IDX			0
#define LORA_RX_MSG_TYPE_IDX			1

//CONTROL VALUES FOR MOTOR PWM VALUES
#define LORA_RX_MOT_SOL_CONTROL_IDX			2	// Sol analog dikey eksen - sadece motor thrust
#define LORA_RX_MOT_SAG_Y_CONTROL_IDX		3	// Sag analog dikey eksen
#define LORA_RX_MOT_SAG_D_CONTROL_IDX		4	// Sag analog yatay eksen
#define LORA_RX_MOT_L1						5	// Sol analog dikey eksen - sadece motor thrust
#define LORA_RX_MOT_R1						6	// Sag analog dikey eksen
#define LORA_RX_MOT_A						7	// Sag analog yatay eksen

//DRONE ID ADDRESS
#define DRONE_ID 0x49	//Gelen mesaj�n ilk byte'� drone ID bilgisidir (ADDRESS BYTE)

//LORA MESSAGE FORMATS
typedef enum{
	ADVERTISING_DATA	= 0x00,
	PERIODIC_DATA		= 0x0A,	//Kumanda datalar� ve di�er normal datalar�n oldu�u mesaj
	CRITICAL_DATA		= 0x0B
}lora_msg_type_t;

typedef enum{
	CTRL_MOTOR_PWM_VALUES 		= 0,
	CTRL_SPECIAL_MOTOR_MOVEMENT	= 1,
	CTRL_OTHER_MESSAGES			= 2
}lora_msg_cntrl_t;

typedef enum{
	STAY_STILL	= 0,
	TURN_RIGHT	= 1,
	TURN_LEFT	= 2,
	TAKLA_AT	= 3
}lora_special_motor_movement_t;


/*************************************
 * DRONEUN ���NDE BULUNDU�U DURUMLAR
 *************************************/
typedef enum{
	STATE_INITIAL, //Power�n ilk verildi�i b�t�n motorlar�n kapal� oldu�u durum
	STATE_MOTORS_OFF,
	STATE_ACTIVELY_CONTROLLED_BY_USER,
	STATE_STAYING_STILL,
	STATE_TURNING_RIGHT,
	STATE_TURNING_LEFT,
	STATE_TAKLA_ATARKEN,
	STATE_RETURNING_HOME
}drone_current_state_t;

typedef struct{
	uint8_t current_state;
	uint8_t thrust;
	volatile uint32_t *mot1_pwm;
	volatile uint32_t *mot2_pwm;
	volatile uint32_t *mot3_pwm;
	volatile uint32_t *mot4_pwm;
}Drone;
#endif /* INC_DRONE_INFOS_H_ */
