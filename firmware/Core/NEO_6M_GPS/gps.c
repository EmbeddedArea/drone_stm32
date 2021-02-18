/**
 ********************************************************************************
 * @file    gps.c
 * @author  Embedded Area
 * @date    Jan 9, 2021
 * @brief
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "gps.h"
#include <stdint.h>
#include <math.h>
#include <stdio.h>

/************************************
 * EXTERN VARIABLES
 ************************************/

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/

/************************************
 * PRIVATE TYPEDEFS
 ************************************/

/************************************
 * STATIC VARIABLES
 ************************************/

#if GPRMC_FRAME_PARSE_ON
static const gps_infoType_t typeTableGPRMC[GPRMC_FRAME_BUFFER_SIZE] = {
		GPS_FLOAT_TYPE,
		GPS_CHARACTER_TYPE,
		GPS_FLOAT_TYPE,
		GPS_CHARACTER_TYPE,
		GPS_FLOAT_TYPE,
		GPS_CHARACTER_TYPE,
		GPS_FLOAT_TYPE,
		GPS_FLOAT_TYPE,
		GPS_INTEGER_TYPE,
		GPS_INTEGER_TYPE,
		GPS_CHARACTER_TYPE,
		GPS_CHARACTER_TYPE
};
#endif

#if GPVTG_FRAME_PARSE_ON

static const gps_infoType_t typeTableGPVTG[GPVTG_FRAME_BUFFER_SIZE] = {
		GPS_INTEGER_TYPE,
		GPS_CHARACTER_TYPE,
		GPS_INTEGER_TYPE,
		GPS_CHARACTER_TYPE,
		GPS_INTEGER_TYPE,
		GPS_CHARACTER_TYPE,
		GPS_INTEGER_TYPE,
		GPS_CHARACTER_TYPE,
		GPS_CHARACTER_TYPE
};
#endif

#if GPGGA_FRAME_PARSE_ON
static const gps_infoType_t typeTableGPGGA[GPGGA_FRAME_BUFFER_SIZE] = {
		GPS_FLOAT_TYPE,
		GPS_FLOAT_TYPE,
		GPS_CHARACTER_TYPE,
		GPS_FLOAT_TYPE,
		GPS_CHARACTER_TYPE,
		GPS_INTEGER_TYPE,
		GPS_INTEGER_TYPE,
		GPS_INTEGER_TYPE,
		GPS_INTEGER_TYPE,
		GPS_CHARACTER_TYPE,
		GPS_INTEGER_TYPE,
		GPS_CHARACTER_TYPE,
		GPS_INTEGER_TYPE,
		GPS_INTEGER_TYPE
};
#endif

#if GPGSA_FRAME_PARSE_ON
static const gps_infoType_t typeTableGPGSA[GPGSA_FRAME_BUFFER_SIZE] = {
		GPS_CHARACTER_TYPE,
		GPS_INTEGER_TYPE,
		GPS_INTEGER_TYPE,
		GPS_INTEGER_TYPE,
		GPS_INTEGER_TYPE,
		GPS_INTEGER_TYPE,
		GPS_INTEGER_TYPE,
		GPS_INTEGER_TYPE,
		GPS_INTEGER_TYPE,
		GPS_INTEGER_TYPE,
		GPS_INTEGER_TYPE,
		GPS_INTEGER_TYPE,
		GPS_INTEGER_TYPE,
		GPS_INTEGER_TYPE,
		GPS_INTEGER_TYPE,
		GPS_INTEGER_TYPE,
		GPS_INTEGER_TYPE
};
#endif

#if GPGSV_FRAME_PARSE_ON
static const gps_infoType_t typeTableGPGSV[GPGSV_FRAME_BUFFER_SIZE] = {
		GPS_INTEGER_TYPE,
		GPS_INTEGER_TYPE,
		GPS_INTEGER_TYPE,
		GPS_INTEGER_TYPE,
		GPS_INTEGER_TYPE,
		GPS_INTEGER_TYPE,
		GPS_INTEGER_TYPE
};

#endif

#if GPGLL_FRAME_PARSE_ON
static const gps_infoType_t typeTableGPGLL[GPGLL_FRAME_BUFFER_SIZE] = {
		GPS_FLOAT_TYPE,
		GPS_CHARACTER_TYPE,
		GPS_FLOAT_TYPE,
		GPS_CHARACTER_TYPE,
		GPS_FLOAT_TYPE,
		GPS_CHARACTER_TYPE,
		GPS_CHARACTER_TYPE
};
#endif

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
 *
 * @param str
 * @param size
 * @param startIndex
 * @param value
 * @return
 */
size_t gps_findIndex(uint8_t * str, size_t size, size_t startIndex, uint8_t value){

	for(size_t i = startIndex; i < size; i++){
		if(str[i] == value){
			return i;
		}
	}
	return GPS_ERROR;
}

/**
 *
 * @param str
 * @param size
 * @param startIndex
 * @param value
 * @param Xth
 * @return
 */
size_t gps_findXthIndex(uint8_t * str, size_t size, size_t startIndex, uint8_t value, uint8_t Xth){

	size_t nextStartIndex = startIndex;
	uint8_t count = 0;
	do{
		nextStartIndex = gps_findIndex(str, size, nextStartIndex, value);
		if(GPS_ERROR == nextStartIndex){
			return GPS_ERROR;	
		}			
		++count;
		++nextStartIndex;	
	}while(count != Xth);	

	return nextStartIndex - 1;	
}

/**
 *
 * @param str
 * @param size
 * @param startIndex
 * @return
 */
gps_Status_t gps_controlChecksum(uint8_t * str, size_t size, size_t startIndex){

	uint32_t checksum = 0;

	for(size_t i = startIndex + 1; i < size; i++){
		if(str[i] == CHECKSUM_INDICATOR){
			return ((checksum & 0xFF) == (((str[i + 1] - '0')*16) + (str[i + 2]- '0') )) ? GPS_VALID_CHECKSUM : GPS_INVALID_CHECKSUM;		
		}
		checksum ^= str[i];		
	}
	return GPS_NO_CHECKSUM_INDICATOR;
}

/**
 *
 * @param str
 * @param size
 * @param startIndex
 * @param address
 * @return
 */
int32_t gps_findAdress(uint8_t * str, size_t size, size_t startIndex, gps_addressIdentifier_t address){

	size_t i = startIndex;
	gps_addressIdentifier_t result;
	do{
		i = gps_findIndex(str, size, i, ADDRESS_INDICATOR);
		if((GPS_ERROR == i) || (i + 5 > size)){ //TODO:different error returns
			return GPS_ERROR;	
		}	
		result = str[i + 3] + str[i + 4] + str[i + 5];
		++i;
	}while(address != result);		

	return (i - 1);
}

/**
 *
 * @param str
 * @param startIndex
 * @param endIndex
 * @return
 */
uint32_t gps_ascii2num(uint8_t * str, size_t startIndex, size_t endIndex){

	uint32_t sum = 0;
	for(size_t i = 0; i < endIndex - startIndex + 1 ; i++){
		sum += (str[endIndex - i] - '0') * pow(10, i);
	}
	return sum;

}
/**
 *
 * @param str
 * @param startIndex
 * @param endIndex
 * @return
 */
float gps_floatAscii2num(uint8_t * str, size_t startIndex, size_t endIndex){

	float sum = 0.0f;
	uint8_t float_flag = 0, float_index;

	for(size_t i = 0; i < endIndex - startIndex + 1 ; i++){
		if(str[endIndex - i] == FLOAT_SEPERATION_INDICATOR){
			float_flag = 1;
			float_index = i;
			sum = (float) (sum * ((float) pow(10, (int) (-i))));
		}			
		else{
			if(!float_flag){
				sum = sum + (float) ((str[endIndex - i] - '0') * pow(10, i));
			}
			else{
				sum = sum + (float) ((str[endIndex - i] - '0') * pow(10, i - 1 - float_index));
			}
		}

	}

	return (float) sum;
}

/**
 *
 * @param str
 * @param index
 * @return
 */
uint8_t gps_getSingleChar(uint8_t * str, size_t index){
	return str[index];
}

/**
 *
 * @param address
 * @return
 */
gps_infoType_t * gps_getTable(gps_addressIdentifier_t address){
	switch(address){
	case GPS_GPRMC:
		return (gps_infoType_t *) typeTableGPRMC;
		break;
	case GPS_GPVTG:
		return (gps_infoType_t *) typeTableGPVTG;
		break;
	case GPS_GPGGA:
		return (gps_infoType_t *) typeTableGPGGA;
		break;
	case GPS_GPGSA:
		return (gps_infoType_t *) typeTableGPGSA;
		break;
	case GPS_GPGSV:
		return (gps_infoType_t *) typeTableGPGSV;
		break;
	case GPS_GPGLL:
		return (gps_infoType_t *) typeTableGPGLL;
		break;									
	default:
		return NULL;
		break;	
	}
}

/**
 *
 * @param str
 * @param startIndex
 * @param endIndex
 * @param address
 * @param index
 * @return
 */
gps_infoData_t gps_getData(uint8_t * str, size_t startIndex, size_t endIndex, gps_infoType_t * address, uint8_t index){

	gps_infoData_t tmp;

	switch(address[index - 1]){
	case GPS_FLOAT_TYPE:	
		tmp.typeOfData = GPS_FLOAT_TYPE;
		tmp.typesOfData.fdata = gps_floatAscii2num(str, startIndex + 1, endIndex - 1);
		break;
	case GPS_INTEGER_TYPE:
		tmp.typeOfData = GPS_INTEGER_TYPE;
		tmp.typesOfData.udata = gps_ascii2num(str, startIndex + 1, endIndex - 1);
		break;
	case GPS_CHARACTER_TYPE:
		tmp.typeOfData = GPS_CHARACTER_TYPE;
		tmp.typesOfData.cdata = gps_getSingleChar(str, startIndex + 1);
		break;			
	default:
		tmp.typeOfData = GPS_INFO_ERROR;		
		break;	
	}
	return tmp;
}

/**
 *
 * @param str
 * @param size
 * @param address
 * @param index
 * @return
 */
gps_infoData_t gps_getInfo(uint8_t * str, size_t size, gps_addressIdentifier_t address, uint8_t index){

	size_t firstIndex, lastIndex; 

	firstIndex = gps_findAdress(str, size, 0, address);	
	/*	if(GPS_VALID_CHECKSUM != gps_controlChecksum(str, size, firstIndex)){
			gps_infoData_t tmp;
			tmp.typeOfData = GPS_INFO_ERROR;
			return tmp;
	}
	 */
	firstIndex = gps_findXthIndex(str, size, firstIndex, VALUE_SEPERATOR_HEXADECIMAL, index);
	lastIndex = gps_findIndex(str, size, firstIndex + 1, VALUE_SEPERATOR_HEXADECIMAL);		
	gps_infoType_t * addressTable = gps_getTable(address);
	if(NULL != addressTable)
		return gps_getData(str, firstIndex, lastIndex, addressTable, index);	
	else{
		gps_infoData_t tmp;
		tmp.typeOfData = GPS_INFO_ERROR;
		return tmp;
	}
}

