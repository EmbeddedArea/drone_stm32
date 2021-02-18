/**
 ********************************************************************************
 * @file    gps_conf.h
 * @author  Embedded Area
 * @date    Jan 9, 2021
 * @brief
 ********************************************************************************
 */

#ifndef INC_GPS_CONF_H_
#define INC_GPS_CONF_H_

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/

/************************************
 * MACROS AND DEFINES
 ************************************/
#define GPRMC_FRAME_PARSE_ON_DEFINED
#ifdef GPRMC_FRAME_PARSE_ON_DEFINED
#define GPRMC_FRAME_PARSE_ON 1
#else
#define GPRMC_FRAME_PARSE_ON 0
#endif

#define GPVTG_FRAME_PARSE_ON_DEFINED
#ifdef GPVTG_FRAME_PARSE_ON_DEFINED
#define GPVTG_FRAME_PARSE_ON 1
#else
#define GPVTG_FRAME_PARSE_ON 0
#endif

#define GPGGA_FRAME_PARSE_ON_DEFINED
#ifdef GPGGA_FRAME_PARSE_ON_DEFINED
#define GPGGA_FRAME_PARSE_ON 1
#else
#define GPGGA_FRAME_PARSE_ON 0
#endif

#define GPGSA_FRAME_PARSE_ON_DEFINED
#ifdef GPGSA_FRAME_PARSE_ON_DEFINED
#define GPGSA_FRAME_PARSE_ON 1
#else
#define GPGSA_FRAME_PARSE_ON 0
#endif

#define GPGSV_FRAME_PARSE_ON_DEFINED
#ifdef GPGSV_FRAME_PARSE_ON_DEFINED
#define GPGSV_FRAME_PARSE_ON 1
#else
#define GPGSV_FRAME_PARSE_ON 0
#endif

#define GPGLL_FRAME_PARSE_ON_DEFINED
#ifdef GPGLL_FRAME_PARSE_ON_DEFINED
#define GPGLL_FRAME_PARSE_ON 1
#else
#define GPGLL_FRAME_PARSE_ON 0
#endif

/************************************
 * TYPEDEFS
 ************************************/

/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/


#ifdef __cplusplus
}
#endif

#endif
