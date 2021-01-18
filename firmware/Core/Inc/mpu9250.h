/*
 * mpu.h
 *
 *  Created on: 10 Oca 2021
 *      Author: Azad
 */

#ifndef INC_MPU_H_
#define INC_MPU_H_

#ifdef __cplusplus
extern "C" {
#endif


/*********************
 *      INCLUDES
 *********************/
#include "main.h"
#include "stdint.h"
#include "cmsis_os.h"

extern I2C_HandleTypeDef hi2c1;


/**********************
 *      MACROS and DEFINES
 **********************/
#define delay(x) (osDelay(x))

#define MPU9250_I2C			hi2c1
#define MPU_I2C_ADDRESS		0x68<<1


/**********************
 *      TYPEDEFS
 **********************/

/**
 * Gyroscope Features
 */
typedef enum
{
	GYRO_RANGE_250DPS,
	GYRO_RANGE_500DPS,
	GYRO_RANGE_1000DPS,
	GYRO_RANGE_2000DPS
} mpu_gyroscope_range_t;

/**
 * Accelerometer Features
 */
typedef enum
{
	ACCEL_RANGE_2G,
	ACCEL_RANGE_4G,
	ACCEL_RANGE_8G,
	ACCEL_RANGE_16G
} mpu_accelerometer_range_t;

/**
 * Bandwidth Values
 */
typedef enum
{
	DLPF_BANDWIDTH_184HZ,
	DLPF_BANDWIDTH_92HZ,
	DLPF_BANDWIDTH_41HZ,
	DLPF_BANDWIDTH_20HZ,
	DLPF_BANDWIDTH_10HZ,
	DLPF_BANDWIDTH_5HZ
} mpu_dlpf_bandwidth_t;

/**
 * Sample Rate Divider Values
 */
typedef enum
{
	LP_ACCEL_ODR_0_24HZ = 0,
	LP_ACCEL_ODR_0_49HZ = 1,
	LP_ACCEL_ODR_0_98HZ = 2,
	LP_ACCEL_ODR_1_95HZ = 3,
	LP_ACCEL_ODR_3_91HZ = 4,
	LP_ACCEL_ODR_7_81HZ = 5,
	LP_ACCEL_ODR_15_63HZ = 6,
	LP_ACCEL_ODR_31_25HZ = 7,
	LP_ACCEL_ODR_62_50HZ = 8,
	LP_ACCEL_ODR_125HZ = 9,
	LP_ACCEL_ODR_250HZ = 10,
	LP_ACCEL_ODR_500HZ = 11
} mpu_sample_rate_divider_t;



/*********************
 *      DEFINES
 *********************/
// MPU9250 registers
#define ACCEL_OUT   0x3B
#define GYRO_OUT   0x43
#define TEMP_OUT   0x41
#define EXT_SENS_DATA_00   0x49
#define ACCEL_CONFIG   0x1C
#define ACCEL_FS_SEL_2G   0x00
#define ACCEL_FS_SEL_4G   0x08
#define ACCEL_FS_SEL_8G   0x10
#define ACCEL_FS_SEL_16G   0x18
#define GYRO_CONFIG   0x1B
#define GYRO_FS_SEL_250DPS   0x00
#define GYRO_FS_SEL_500DPS   0x08
#define GYRO_FS_SEL_1000DPS   0x10
#define GYRO_FS_SEL_2000DPS   0x18
#define ACCEL_CONFIG2   0x1D
#define ACCEL_DLPF_184   0x01
#define ACCEL_DLPF_92   0x02
#define ACCEL_DLPF_41   0x03
#define ACCEL_DLPF_20   0x04
#define ACCEL_DLPF_10   0x05
#define ACCEL_DLPF_5   0x06
#define CONFIG   0x1A
#define GYRO_DLPF_184   0x01
#define GYRO_DLPF_92   0x02
#define GYRO_DLPF_41   0x03
#define GYRO_DLPF_20   0x04
#define GYRO_DLPF_10   0x05
#define GYRO_DLPF_5   0x06
#define SMPDIV   0x19
#define INT_PIN_CFG   0x37
#define INT_ENABLE   0x38
#define INT_DISABLE   0x00
#define INT_PULSE_50US   0x00
#define INT_WOM_EN   0x40
#define INT_RAW_RDY_EN   0x01
#define PWR_MGMNT_1   0x6B
#define PWR_CYCLE   0x20
#define PWR_RESET   0x80
#define CLOCK_SEL_PLL   0x01
#define PWR_MGMNT_2   0x6C
#define SEN_ENABLE   0x00
#define DIS_GYRO   0x07
#define USER_CTRL   0x6A
#define I2C_MST_EN   0x20
#define I2C_MST_CLK   0x0D
#define I2C_MST_CTRL   0x24
#define I2C_SLV0_ADDR   0x25
#define I2C_SLV0_REG   0x26
#define I2C_SLV0_DO   0x63
#define I2C_SLV0_CTRL   0x27
#define I2C_SLV0_EN   0x80
#define I2C_READ_FLAG   0x80
#define MOT_DETECT_CTRL   0x69
#define ACCEL_INTEL_EN   0x80
#define ACCEL_INTEL_MODE   0x40
#define LP_ACCEL_ODR   0x1E
#define WOM_THR   0x1F
#define WHO_AM_I   0x75
#define FIFO_EN   0x23
#define FIFO_TEMP   0x80
#define FIFO_GYRO   0x70
#define FIFO_ACCEL   0x08
#define FIFO_MAG   0x01
#define FIFO_COUNT   0x72
#define FIFO_READ   0x74

// AK8963 registers
#define AK8963_I2C_ADDR   	0x0C
#define AK8963_ST1   		0x02
#define AK8963_HXL   		0x03
#define AK8963_CNTL1   		0x0A
#define AK8963_PWR_DOWN   	0x00
#define AK8963_CNT_MEAS1   	0x12
#define AK8963_CNT_MEAS2   	0x16
#define AK8963_FUSE_ROM   	0x0F
#define AK8963_CNTL2   		0x0B
#define AK8963_RESET   		0x01
#define AK8963_ASA   		0x10
#define AK8963_WHO_AM_I   	0x00


/*************************
 *  FUNCTION PROTOTYPES
 *************************/
int whoAmI();
int whoAmIAK8963();

int mpu_init();
int setAccelRange(mpu_accelerometer_range_t range);
int setGyroRange(mpu_gyroscope_range_t range);
int setDlpfBandwidth(mpu_dlpf_bandwidth_t bandwidth);
int setSrd(uint8_t srd);

int readSensor();
float getAccelX_mss();
float getAccelY_mss();
float getAccelZ_mss();
float getGyroX_rads();
float getGyroY_rads();
float getGyroZ_rads();
float getMagX_uT();
float getMagY_uT();
float getMagZ_uT();
float getTemperature_C();

int calibrateGyro();
int calibrateMagneto(void) ;

float getGyroBiasX_rads();
float getGyroBiasY_rads();
float getGyroBiasZ_rads();
void setGyroBiasX_rads(float bias);
void setGyroBiasY_rads(float bias);
void setGyroBiasZ_rads(float bias);

#ifdef __cplusplus
}
#endif
#endif /* INC_MPU_H_ */
