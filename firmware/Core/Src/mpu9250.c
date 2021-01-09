/**
 ******************************************************************************
 * @file        mpu9250.c
 * @brief       Contains MPU9250 IMU driver and related functions
 * @author 		Azad Karataş
 ******************************************************************************
 */

/*********************
 *      INCLUDES
 *********************/
#include "mpu9250.h"


static uint8_t _mag_adjust[3];
static uint8_t dummyBufferToRW[21];

/*********************
 *  STATIC FUNCTIONS
 *********************/

/**
 *
 */
static void spi_enable()
{
	HAL_GPIO_WritePin(MPU9250_CS_GPIO, MPU9250_CS_PIN, GPIO_PIN_RESET);
}

/**
 *
 */
static void spi_disable(){
	HAL_GPIO_WritePin(MPU9250_CS_GPIO, MPU9250_CS_PIN, GPIO_PIN_SET);
}

/**
 *
 * @param address
 * @param dataBuffer
 * @param number_of_bytes
 */
static void spi_write (uint8_t address, uint8_t *dataBuffer, uint8_t number_of_bytes){
	uint8_t data_to_send[10];
	data_to_send[0] = address;
	for(int i = 0; i < number_of_bytes; i++){
		data_to_send[i + 1] = dataBuffer[i];
	}
	spi_enable();
	HAL_SPI_Transmit(&MPU9250_SPI, data_to_send, number_of_bytes, HAL_MAX_DELAY);
	spi_disable();
}

/**
 *
 * @param address
 * @param dataBuffer
 * @param number_of_bytes
 */
static void spi_read(uint8_t address, uint8_t *dataBuffer, uint8_t number_of_bytes){
	spi_enable();
	uint8_t data = address | 0x80;
	HAL_SPI_Transmit(&MPU9250_SPI, &data, 1, 0xFF);
	HAL_SPI_Receive(&MPU9250_SPI, dataBuffer, number_of_bytes, 0xFF);
	spi_disable();
}

/**
 *
 * @param subAddress
 * @param data
 */
/* writes a byte to MPU9250 register given a register address and data */
void writeRegister(uint8_t subAddress, uint8_t data){
	spi_write(subAddress, &data, 1);
	HAL_Delay(10);
}

/* reads registers from MPU9250 given a starting register address, number of bytes, and a pointer to store data */
void readRegisters(uint8_t subAddress, uint8_t count, uint8_t* dest){
	spi_read(subAddress, dest, count);
}

/* writes a register to the AK8963 given a register address and data */
void writeAK8963Register(uint8_t subAddress, uint8_t data){
	// set slave 0 to the AK8963 and set for write
	writeRegister(I2C_SLV0_ADDR,AK8963_ADDRESS);

	// set the register to the desired AK8963 sub address
	writeRegister(I2C_SLV0_REG,subAddress);

	// store the data for write
	writeRegister(I2C_SLV0_DO,data);

	// enable I2C and send 1 byte
	writeRegister(I2C_SLV0_CTRL,I2C_SLV0_EN | (uint8_t)1);
}

/* reads registers from the AK8963 */
void readAK8963Registers(uint8_t subAddress, uint8_t count, uint8_t* dest){
	// set slave 0 to the AK8963 and set for read
	writeRegister(I2C_SLV0_ADDR, AK8963_ADDRESS | I2C_SLV0_EN);

	// set the register to the desired AK8963 sub address
	writeRegister(I2C_SLV0_REG,subAddress);

	// enable I2C and request the bytes
	writeRegister(I2C_SLV0_CTRL,I2C_SLV0_EN | count);

	// takes some time for these registers to fill
	HAL_Delay(1);

	// read the bytes off the MPU9250 EXT_SENS_DATA registers
	readRegisters(EXT_SENS_DATA_00,count,dest);
}

/* gets the MPU9250 WHO_AM_I register value, expected to be 0x71 */
static uint8_t whoAmI(){
	// read the WHO AM I register
	readRegisters(WHO_AM_I_MPU9250, 1, dummyBufferToRW);

	// return the register value
	return dummyBufferToRW[0];
}

/* gets the AK8963 WHO_AM_I register value, expected to be 0x48 */
static int whoAmIAK8963(){
	// read the WHO AM I register
	readAK8963Registers(AK8963_WHO_AM_I,1,dummyBufferToRW);
	// return the register value
	return dummyBufferToRW[0];
}

/* starts communication with the MPU-9250 */
uint8_t MPU_Init(){
	// select clock source to gyro
	writeRegister(PWR_MGMT_1, CLOCK_SEL_PLL);
	HAL_UART_Transmit(&HUART_PC, (uint8_t *) "b", 1, 0xFF);
	// enable I2C master mode
	writeRegister(USER_CTRL, I2C_MST_EN);
	// set the I2C bus speed to 400 kHz
	writeRegister(I2C_MST_CTRL, I2C_MST_CLK);

	// set AK8963 to Power Down
	writeAK8963Register(AK8963_CNTL,AK8963_PWR_DOWN);
	// reset the MPU9250
	writeRegister(PWR_MGMT_1,PWR_RESET);
	// wait for MPU-9250 to come back up
	HAL_Delay(10);
	// reset the AK8963
	writeAK8963Register(AK8963_CNTL2,AK8963_RESET);
	// select clock source to gyro
	writeRegister(PWR_MGMT_1,CLOCK_SEL_PLL);

	// check the WHO AM I byte, expected value is 0x71 (decimal 113) or 0x73 (decimal 115)
	uint8_t who = whoAmI();
	if((who != 0x71) &&( who != 0x73))
	{
		return 1;
	}

	// enable accelerometer and gyro
	writeRegister(PWR_MGMT_2,SEN_ENABLE);

	// setting accel range to 16G as default
	writeRegister(ACCEL_CONFIG,ACCEL_FS_SEL_16G);

	// setting the gyro range to 2000DPS as default
	writeRegister(GYRO_CONFIG,GYRO_FS_SEL_250DPS);

	// setting bandwidth to 184Hz as default
	writeRegister(ACCEL_CONFIG2,DLPF_184);

	// setting gyro bandwidth to 184Hz
	writeRegister(CONFIG,DLPF_184);

	// setting the sample rate divider to 0 as default
	writeRegister(SMPDIV,0x00);

	// enable I2C master mode
	writeRegister(USER_CTRL,I2C_MST_EN);

	// set the I2C bus speed to 400 kHz
	writeRegister(I2C_MST_CTRL,I2C_MST_CLK);

	// check AK8963 WHO AM I register, expected value is 0x48 (decimal 72)
	if( whoAmIAK8963() != 0x48 )
	{
		return 1;
	}

	/* get the magnetometer calibration */
	// set AK8963 to Power Down
	writeAK8963Register(AK8963_CNTL1,AK8963_PWR_DOWN);

	HAL_Delay(100); // long wait between AK8963 mode changes

	// set AK8963 to FUSE ROM access
	writeAK8963Register(AK8963_CNTL1,AK8963_FUSE_ROM);

	// long wait between AK8963 mode changes
	HAL_Delay(100);

	// read the AK8963 ASA registers and compute magnetometer scale factors
	readAK8963Registers(AK8963_ASA, 3, _mag_adjust);

	// set AK8963 to Power Down
	writeAK8963Register(AK8963_CNTL1,AK8963_PWR_DOWN);

	// long wait between AK8963 mode changes
	HAL_Delay(100);

	// set AK8963 to 16 bit resolution, 100 Hz update rate
	writeAK8963Register(AK8963_CNTL1,AK8963_CNT_MEAS2);

	// long wait between AK8963 mode changes
	HAL_Delay(100);

	// select clock source to gyro
	writeRegister(PWR_MGMT_1,CLOCK_SEL_PLL);

	// instruct the MPU9250 to get 7 bytes of data from the AK8963 at the sample rate
	readAK8963Registers(AK8963_HXL,7,dummyBufferToRW);

	// successful init, return 0
	return 0;
}

/**MPU9250_GetData
 * Reads the sensor data
 * @param AccData: Accelerometer data buffer address
 * @param MagData: Magnetometer data buffer address
 * @param GyroData: Gyroscope data buffer address
 * @param Temperature: Temperature data buffer address
 */
/* read the data, each argiment should point to a array for x, y, and x */
void MPU_GetData(int16_t* AccData, int16_t* MagData, int16_t* GyroData, int16_t *Temperature){
	/* Starts reading 21 bytes data from first address (ACCEL_XOUT_H) */
	readRegisters(ACCEL_XOUT_H, 21, dummyBufferToRW);

	/* Parsing accelerometer data */
	AccData[0] = (((int16_t)dummyBufferToRW[0]) << 8) | dummyBufferToRW[1];
	AccData[1] = (((int16_t)dummyBufferToRW[2]) << 8) | dummyBufferToRW[3];
	AccData[2] = (((int16_t)dummyBufferToRW[4]) << 8) | dummyBufferToRW[5];

	/* Parsing temperature data */
	*Temperature = (((int16_t)dummyBufferToRW[6]) << 8) | dummyBufferToRW[7];

	/* Parsing gyroscope data */
	GyroData[0] = (((int16_t)dummyBufferToRW[8]) << 8) | dummyBufferToRW[9];
	GyroData[1] = (((int16_t)dummyBufferToRW[10]) << 8) | dummyBufferToRW[11];
	GyroData[2] = (((int16_t)dummyBufferToRW[12]) << 8) | dummyBufferToRW[13];

	/* Parsing magnetometer data */
	int16_t magx = (((int16_t)dummyBufferToRW[15]) << 8) | dummyBufferToRW[14];
	int16_t magy = (((int16_t)dummyBufferToRW[17]) << 8) | dummyBufferToRW[16];
	int16_t magz = (((int16_t)dummyBufferToRW[19]) << 8) | dummyBufferToRW[18];

	MagData[0] = (int16_t)((float)magx * ((float)(_mag_adjust[0] - 128) / 256.0f + 1.0f));
	MagData[1] = (int16_t)((float)magy * ((float)(_mag_adjust[1] - 128) / 256.0f + 1.0f));
	MagData[2] = (int16_t)((float)magz * ((float)(_mag_adjust[2] - 128) / 256.0f + 1.0f));
}

void MPU_Reset(void){
	writeRegister(PWR_MGMT_1, 0x80);
	HAL_Delay(100);
}



// parameters for 6 DoF sensor fusion calculations
float PI = 3.14159265358979323846f;
float GyroMeasError = 1.047198f;     // gyroscope measurement error in rads/s (start at 60 deg/s), then reduce after ~10 s to 3
float beta = 0.9069f;  // compute beta
float GyroMeasDrift = 0.017453f;      // gyroscope measurement drift in rad/s/s (start at 0.0 deg/s/s)
float zeta = 0.015115f;  // compute zeta, the other free parameter in the Madgwick scheme usually set to a small or zero value


float pitch, yaw, roll;
float deltat = 0.0f;                             // integration interval for both filter schemes
int lastUpdate = 0, firstUpdate = 0, Now = 0;    // used to calculate integration interval                               // used to calculate integration interval
float q[4] = {1.0f, 0.0f, 0.0f, 0.0f};           // vector to hold quaternion
float eInt[3] = {0.0f, 0.0f, 0.0f};              // vector to hold integral error for Mahony method


// Implementation of Sebastian Madgwick's "...efficient orientation filter for... inertial/magnetic sensor arrays"
// (see http://www.x-io.co.uk/category/open-source/ for examples and more details)
// which fuses acceleration, rotation rate, and magnetic moments to produce a quaternion-based estimate of absolute
// device orientation -- which can be converted to yaw, pitch, and roll. Useful for stabilizing quadcopters, etc.
// The performance of the orientation filter is at least as good as conventional Kalman-based filtering algorithms
// but is much less computationally intensive---it can be performed on a 3.3 V Pro Mini operating at 8 MHz!
void MadgwickQuaternionUpdate(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz)
{
	float q1 = q[0], q2 = q[1], q3 = q[2], q4 = q[3];   // short name local variable for readability
	float norm;
	float hx, hy, _2bx, _2bz;
	float s1, s2, s3, s4;
	float qDot1, qDot2, qDot3, qDot4;

	// Auxiliary variables to avoid repeated arithmetic
	float _2q1mx;
	float _2q1my;
	float _2q1mz;
	float _2q2mx;
	float _4bx;
	float _4bz;
	float _2q1 = 2.0f * q1;
	float _2q2 = 2.0f * q2;
	float _2q3 = 2.0f * q3;
	float _2q4 = 2.0f * q4;
	float _2q1q3 = 2.0f * q1 * q3;
	float _2q3q4 = 2.0f * q3 * q4;
	float q1q1 = q1 * q1;
	float q1q2 = q1 * q2;
	float q1q3 = q1 * q3;
	float q1q4 = q1 * q4;
	float q2q2 = q2 * q2;
	float q2q3 = q2 * q3;
	float q2q4 = q2 * q4;
	float q3q3 = q3 * q3;
	float q3q4 = q3 * q4;
	float q4q4 = q4 * q4;

	// Normalise accelerometer measurement
	norm = sqrt(ax * ax + ay * ay + az * az);
	if (norm == 0.0f) return; // handle NaN
	norm = 1.0f/norm;
	ax *= norm;
	ay *= norm;
	az *= norm;

	// Normalise magnetometer measurement
	norm = sqrt(mx * mx + my * my + mz * mz);
	if (norm == 0.0f) return; // handle NaN
	norm = 1.0f/norm;
	mx *= norm;
	my *= norm;
	mz *= norm;

	// Reference direction of Earth's magnetic field
	_2q1mx = 2.0f * q1 * mx;
	_2q1my = 2.0f * q1 * my;
	_2q1mz = 2.0f * q1 * mz;
	_2q2mx = 2.0f * q2 * mx;
	hx = mx * q1q1 - _2q1my * q4 + _2q1mz * q3 + mx * q2q2 + _2q2 * my * q3 + _2q2 * mz * q4 - mx * q3q3 - mx * q4q4;
	hy = _2q1mx * q4 + my * q1q1 - _2q1mz * q2 + _2q2mx * q3 - my * q2q2 + my * q3q3 + _2q3 * mz * q4 - my * q4q4;
	_2bx = sqrt(hx * hx + hy * hy);
	_2bz = -_2q1mx * q3 + _2q1my * q2 + mz * q1q1 + _2q2mx * q4 - mz * q2q2 + _2q3 * my * q4 - mz * q3q3 + mz * q4q4;
	_4bx = 2.0f * _2bx;
	_4bz = 2.0f * _2bz;

	// Gradient decent algorithm corrective step
	s1 = -_2q3 * (2.0f * q2q4 - _2q1q3 - ax) + _2q2 * (2.0f * q1q2 + _2q3q4 - ay) - _2bz * q3 * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx) + (-_2bx * q4 + _2bz * q2) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my) + _2bx * q3 * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
	s2 = _2q4 * (2.0f * q2q4 - _2q1q3 - ax) + _2q1 * (2.0f * q1q2 + _2q3q4 - ay) - 4.0f * q2 * (1.0f - 2.0f * q2q2 - 2.0f * q3q3 - az) + _2bz * q4 * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx) + (_2bx * q3 + _2bz * q1) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my) + (_2bx * q4 - _4bz * q2) * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
	s3 = -_2q1 * (2.0f * q2q4 - _2q1q3 - ax) + _2q4 * (2.0f * q1q2 + _2q3q4 - ay) - 4.0f * q3 * (1.0f - 2.0f * q2q2 - 2.0f * q3q3 - az) + (-_4bx * q3 - _2bz * q1) * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx) + (_2bx * q2 + _2bz * q4) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my) + (_2bx * q1 - _4bz * q3) * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
	s4 = _2q2 * (2.0f * q2q4 - _2q1q3 - ax) + _2q3 * (2.0f * q1q2 + _2q3q4 - ay) + (-_4bx * q4 + _2bz * q2) * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx) + (-_2bx * q1 + _2bz * q3) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my) + _2bx * q2 * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
	norm = sqrt(s1 * s1 + s2 * s2 + s3 * s3 + s4 * s4);    // normalise step magnitude
	norm = 1.0f/norm;
	s1 *= norm;
	s2 *= norm;
	s3 *= norm;
	s4 *= norm;

	// Compute rate of change of quaternion
	qDot1 = 0.5f * (-q2 * gx - q3 * gy - q4 * gz) - beta * s1;
	qDot2 = 0.5f * (q1 * gx + q3 * gz - q4 * gy) - beta * s2;
	qDot3 = 0.5f * (q1 * gy - q2 * gz + q4 * gx) - beta * s3;
	qDot4 = 0.5f * (q1 * gz + q2 * gy - q3 * gx) - beta * s4;

	// Integrate to yield quaternion
	q1 += qDot1 * deltat;
	q2 += qDot2 * deltat;
	q3 += qDot3 * deltat;
	q4 += qDot4 * deltat;
	norm = sqrt(q1 * q1 + q2 * q2 + q3 * q3 + q4 * q4);    // normalise quaternion
	norm = 1.0f/norm;
	q[0] = q1 * norm;
	q[1] = q2 * norm;
	q[2] = q3 * norm;
	q[3] = q4 * norm;

}


