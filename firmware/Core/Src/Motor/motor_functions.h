/**
 ********************************************************************************
 * @file    motor_functions.h
 * @author  Embedded Area
 * @date    Feb 24, 2021
 * @brief   
 ********************************************************************************
 */

#ifndef SRC_MOTOR_MOTOR_FUNCTIONS_H_
#define SRC_MOTOR_MOTOR_FUNCTIONS_H_

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/
#include "drone_infos.h"
#include "main.h"

/************************************
 * MACROS AND DEFINES
 ************************************/
//todo: Explain these macros
#define MAX_THRUST_VALUE 		115	//255-140 = 115
#define THRUST_UNITY_COEFF 		(255/MAX_THRUST_VALUE)

#define MAX_DIRECTION_VALUE 	125.0	//255-140 = 115
#define FULL_GAZ				180.0
#define LEFT_RIGHT_LIMIT		25.0	// 50/2
#define YAW_OFFSET				15.0
#define MINIMUM_TICK 			98.0
#define MAXIMUM_TICK 			218.0
#define MS_TO_TICK_CONST		0.91

/************************************
 * TYPEDEFS
 ************************************/

/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/

/**
 * Hold the drone still.
 */
void motor_stay_still(void);

/**
 * Set PWM values to rotate drone right.
 */
void motor_turn_right(void);

/**
  * Set PWM values to rotate drone left.
 */
void motor_turn_left(void);

/**
 * Set motor PWM values according to incoming speed and orientation values
 * @param motor_thrust: motor thrust value
 * @param motor_x_axis: motor movement in X axis
 * @param motor_y_axis: motor movement in Y axis
 */
void motor_active_movement(uint8_t motor_thrust, uint8_t motor_x_axis, uint8_t motor_y_axis);


#ifdef __cplusplus
}
#endif

#endif 
