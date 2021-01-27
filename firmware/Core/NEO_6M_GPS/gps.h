/*
 * gps.h
 *
 *  Created on: Jan 9, 2021
 *      Author: Sefa
 */

#ifndef INC_GPS_H_
#define INC_GPS_H_

#include <stddef.h>
#include <stdint.h>
#include "gps_conf.h"

#define VALUE_SEPERATOR_HEXADECIMAL 		0x2C
#define ADDRESS_INDICATOR					0x24
#define CHECKSUM_INDICATOR					0x2A
#define FLOAT_SEPERATION_INDICATOR			0x2E

typedef enum{
GPS_FLOAT_TYPE = 0,
GPS_INTEGER_TYPE,
GPS_CHARACTER_TYPE,
GPS_INFO_ERROR	
}gps_infoType_t;

typedef struct{
	gps_infoType_t typeOfData;	
	union{
		float fdata;
		uint32_t udata;
		uint8_t cdata;
	}typesOfData;
}gps_infoData_t;

#if GPRMC_FRAME_PARSE_ON
typedef enum GPRMC_FRAME_{
	GPRMC_UTC_TIME = 0,
	GPRMC_STATUS,
	GPRMC_LATITUDE,
	GPRMC_NORTH_OR_SOUTH,
	GPRMC_LONGITUDE,
	GPRMC_EAST_OR_WEST,
	GPRMC_SPEED,
	GPRMC_COURSE,
	GPRMC_DATE,
	GPRMC_MAGNETIC_VARIATION,
	GPRMC_MAGNETIC_EAST_OR_WEST,
	GPRMC_MODE,
	GPRMC_FRAME_BUFFER_SIZE
}dataGPRMC_t;
#endif

#if GPVTG_FRAME_PARSE_ON
typedef enum GPVTG_FRAME_{
	GPVTG_COURSE_OVER_GROUND = 0,
	GPVTG_FIXED_FIELD,
	GPVTG_COURSE_OVER_GROUND_MAGNETIC,
	GPVTG_FIXED_FIELD_MAGNETIC,
	GPVTG_SPEED_OVER_GROUND_KNOTS,
	GPVTG_FIXED_FIELD_KNOTS,
	GPVTG_SPEED_OVER_GROUND_KM_PER_H,
	GPVTG_FIXED_FIELD_KM_PER_H,
	GPVTG_MODE,
	GPVTG_FRAME_BUFFER_SIZE,
}dataGPVTG_t;
#endif

#if GPGGA_FRAME_PARSE_ON
typedef enum GPGGA_FRAME_{
	GPGGA_UTC_TIME = 0,
	GPGGA_LATITUDE,
	GPGGA_NORTH_OR_SOUTH,
	GPGGA_LONGITUDE,
	GPGGA_EAST_OR_WEST,
	GPGGA_POSITION_FIX_STATUS,
	GPGGA_SATELLITES_USED,
	GPGGA_HORIZONTAL_DILUTION_OF_PRECISION,
	GPGGA_MSL_ALTITUDE,
	GPGGA_UNITS_MSL,
	GPGGA_GEOID_SEPERATION,
	GPGGA_UNITS_SEPERATION,
	GPGGA_AGE_OF_DIFFERENTIAL_CORRECTIONS,
	GPGGA_DIFF_REFERENCE_STATION_ID,
	GPGGA_FRAME_BUFFER_SIZE,
}dataGPGGA_t;	
#endif

#if GPGSA_FRAME_PARSE_ON
typedef enum GPGSA_FRAME_{
	GPGSA_S_MODE = 0,
	GPGSA_FIX_STATUS,
	GPGSA_SATELLITE_NUMBER_1,
	GPGSA_SATELLITE_NUMBER_2,
	GPGSA_SATELLITE_NUMBER_3,
	GPGSA_SATELLITE_NUMBER_4,
	GPGSA_SATELLITE_NUMBER_5,
	GPGSA_SATELLITE_NUMBER_6,
	GPGSA_SATELLITE_NUMBER_7,
	GPGSA_SATELLITE_NUMBER_8,
	GPGSA_SATELLITE_NUMBER_9,
	GPGSA_SATELLITE_NUMBER_10,
	GPGSA_SATELLITE_NUMBER_11,
	GPGSA_SATELLITE_NUMBER_12,
	GPGSA_POSITION_DILUTION_OF_PRECISION,
	GPGSA_HORIZONTAL_DILUTION_OF_PRECISION,
	GPGSA_VERTICAL_DILUTION_OF_PRECISION,
	GPGSA_FRAME_BUFFER_SIZE,
}dataGPGSA_t;	
#endif

#if GPGSV_FRAME_PARSE_ON
typedef enum GPGSV_FRAME_{
	GPGSV_TOTAL_NUMBER_OF_MESSAGES = 0,
	GPGSV_NUMBER_OF_RELATED_MESSAGE,
	GPGSV_SATELLITES_IN_VIEW,
	GPGSV_SATELLITE_ID,
	GPGSV_ELEVATION,
	GPGSV_AZIMUTH,
	GPGSV_C_PER_N0,
	GPGSV_FRAME_BUFFER_SIZE,
}dataGPGSV_t;	
#endif

#if GPGLL_FRAME_PARSE_ON
typedef enum GPGLL_FRAME_{
	GPGLL_LATITUDE = 0,
	GPGLL_NORTH_OR_SOUTH,
	GPGLL_LONGITUDE,
	GPGLL_EAST_OR_WEST,
	GPGLL_UTC_TIME,
	GPGLL_STATUS,
	GPGLL_MODE,
	GPGLL_FRAME_BUFFER_SIZE,
}dataGPGLL_t;	
#endif

typedef enum{
	GPS_ERROR 					= -3,
	GPS_NO_CHECKSUM_INDICATOR	= -2,
	GPS_INVALID_CHECKSUM		= -1,
	GPS_VALID_CHECKSUM			=  0,
	GPS_SUCCESSFUL              =  1,
}gps_Status_t;

/* Sum of three bytes sentence formatters to identify */
typedef enum{
	GPS_GPRMC = 'R' + 'M' + 'C',
	GPS_GPVTG = 'V' + 'T' + 'G',
	GPS_GPGGA = 'G' + 'G' + 'A',
	GPS_GPGSA = 'G' + 'S' + 'A',
	GPS_GPGSV = 'G' + 'G' + 'V',
	GPS_GPGLL = 'G' + 'L' + 'L',
}gps_addressIdentifier_t;

size_t gps_findIndex(uint8_t * str, size_t size, size_t startIndex, uint8_t value);
size_t gps_findXthIndex(uint8_t * str, size_t size, size_t startIndex, uint8_t value, uint8_t Xth);
gps_Status_t gps_controlChecksum(uint8_t * str, size_t size, size_t startIndex);
int32_t gps_findAdress(uint8_t * str, size_t size, size_t startIndex, gps_addressIdentifier_t address);
uint32_t gps_ascii2num(uint8_t * str, size_t startIndex, size_t endIndex);
float gps_floatAscii2num(uint8_t * str, size_t startIndex, size_t endIndex);
uint8_t gps_getSingleChar(uint8_t * str, size_t index);
gps_infoType_t * gps_getTable(gps_addressIdentifier_t address);
gps_infoData_t gps_getData(uint8_t * str, size_t startIndex, size_t endIndex, gps_infoType_t * address, uint8_t index);
gps_infoData_t gps_getInfo(uint8_t * str, size_t size, gps_addressIdentifier_t address, uint8_t index);

#endif /* INC_GPS_H_ */

