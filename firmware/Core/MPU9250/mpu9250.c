/*
 * mpu9250.c
 *
 *  Created on: 10 Oca 2021
 *      Author: Azad Karataş
 */

/*********************
 *      INCLUDES
 *********************/
#include "../MPU9250/mpu9250.h"

#include "stdlib.h"
#include "cmsis_os.h"

/**********************
 *  EXTERN VARIABLES
 **********************/
extern osSemaphoreId mpu9265_smphrHandle;

/******************************
 *  GLOBAL & STATIC VARIABLES
 ******************************/
// track success of interacting with sensor
int _status;

// buffer for reading from sensor
uint8_t _buffer[21];

// Raw 16 bit data for MPU sensor outputs
int16_t raw_accel_x, raw_accel_y, raw_accel_z;
int16_t raw_gyro_x, raw_gyro_y, raw_gyro_z;
int16_t raw_magneto_x, raw_magneto_y, raw_magneto_z;
int16_t raw_temperature;

// Base calibration values
float base_x_accel, base_y_accel, base_z_accel, base_x_gyro, base_y_gyro, base_z_gyro;

float angularVelocityX, angularVelocityY;
float aRoll, aPitch, oldRoll, oldPitch;
float dt, alpha;

// data buffer
float accel_x, accel_y, accel_z;
float gyro_x, gyro_y, gyro_z;
float magneto_x, magneto_y, magneto_z;
float tempera;

// scale factors
float _accelScale;
float _gyroScale;
float _magScaleX, _magScaleY, _magScaleZ;
const float _tempScale = 333.87f;
const float _tempOffset = 21.0f;

// configuration
mpu_accelerometer_range_t _accelRange;
mpu_gyroscope_range_t _gyroRange;
mpu_dlpf_bandwidth_t _bandwidth;
uint8_t _srd;

// gyro bias estimation
uint8_t _numSamples = 100;
double _gxbD, _gybD, _gzbD;
float _gxb, _gyb, _gzb;

// accel bias and scale factor estimation
double _axbD, _aybD, _azbD;
float _axmax, _aymax, _azmax;
float _axmin, _aymin, _azmin;
float _axb, _ayb, _azb;
float _axs = 1.0f;
float _ays = 1.0f;
float _azs = 1.0f;

// magnetometer bias and scale factor estimation
uint16_t _maxCounts = 1000;
float _deltaThresh = 0.3f;
uint8_t _coeff = 8;
uint16_t _counter;
float _framedelta, _delta;
float _hxfilt, _hyfilt, _hzfilt;
float _hxmax, _hymax, _hzmax;
float _hxmin, _hymin, _hzmin;
float _hxb, _hyb, _hzb;
float _hxs = 1.0f;
float _hys = 1.0f;
float _hzs = 1.0f;
float _avgs;

// constants


/*************************
 *  STATIC FUNCTION PROTOTYPES
 *************************/
static int writeRegister(uint8_t subAddress, uint8_t data);
static int readRegisters(uint8_t subAddress, uint8_t count, uint8_t* dest);
static int writeAK8963Register(uint8_t subAddress, uint8_t data);
static int readAK8963Registers(uint8_t subAddress, uint8_t count, uint8_t* dest);

/**
 *
 * @param subAddress
 * @param data
 * @return
 */
/* writes a byte to MPU9250 register given a register address and data */
static int writeRegister(uint8_t subAddress, uint8_t data){
	uint8_t data_to_send[2];
	data_to_send[0] = subAddress;
	data_to_send[1] = data;

	HAL_I2C_Master_Transmit(&MPU9250_I2C, MPU_I2C_ADDRESS, data_to_send, 2, 0xFF);

	delay(10);

	/* read back the register */
	readRegisters(subAddress, 1, _buffer);
	/* check the read back register against the written register */
	if(_buffer[0] == data) {
		return 1;
	}
	else{
		return -1;
	}
}

/**readRegisters
 * @param subAddress
 * @param count
 * @param dest
 * @return
 */
static int readRegisters(uint8_t subAddress, uint8_t count, uint8_t* dest){
	HAL_I2C_Master_Transmit(&MPU9250_I2C, MPU_I2C_ADDRESS, &subAddress, 1, 0xFF);
	HAL_I2C_Master_Receive_DMA(&MPU9250_I2C, MPU_I2C_ADDRESS, dest, count);
	osSemaphoreWait(mpu9265_smphrHandle, osWaitForever);
	return 1;
}

/**writeAK8963Register
 * @param subAddress
 * @param data
 * @return
 */
static int writeAK8963Register(uint8_t subAddress, uint8_t data){
	// set slave 0 to the AK8963 and set for write
	if (writeRegister(I2C_SLV0_ADDR, AK8963_I2C_ADDR) < 0) {
		return -1;
	}
	// set the register to the desired AK8963 sub address
	if (writeRegister(I2C_SLV0_REG, subAddress) < 0) {
		return -2;
	}
	// store the data for write
	if (writeRegister(I2C_SLV0_DO, data) < 0) {
		return -3;
	}
	// enable I2C and send 1 byte
	if (writeRegister(I2C_SLV0_CTRL, I2C_SLV0_EN | (uint8_t)1) < 0) {
		return -4;
	}
	// read the register and confirm
	if (readAK8963Registers(subAddress, 1, _buffer) < 0) {
		return -5;
	}
	if(_buffer[0] == data) {
		return 1;
	} else{
		return -6;
	}
}

/**readAK8963Registers
 * @param subAddress
 * @param count
 * @param dest
 * @return
 */
static int readAK8963Registers(uint8_t subAddress, uint8_t count, uint8_t* dest){
	// set slave 0 to the AK8963 and set for read
	if (writeRegister(I2C_SLV0_ADDR, AK8963_I2C_ADDR | I2C_READ_FLAG) < 0) {
		return -1;
	}
	// set the register to the desired AK8963 sub address
	if (writeRegister(I2C_SLV0_REG, subAddress) < 0) {
		return -2;
	}
	// enable I2C and request the bytes
	if (writeRegister(I2C_SLV0_CTRL, I2C_SLV0_EN | count) < 0) {
		return -3;
	}
	delay(1);
	// read the bytes off the MPU9250 EXT_SENS_DATA registers and return
	return readRegisters(EXT_SENS_DATA_00, count, dest);
}


/**MPU9250_WhoAmI
 *
 * @return
 */
int MPU9250_WhoAmI(){
	// read the WHO AM I register
	if (readRegisters(WHO_AM_I,1,_buffer) < 0) {
		return -1;
	}
	// return the register value
	return _buffer[0];
}


/**MPU9250_WhoAmIAK8963
 *
 * @return
 */
int MPU9250_WhoAmIAK8963(){
	// read the WHO AM I register
	if (readAK8963Registers(AK8963_WHO_AM_I,1,_buffer) < 0) {
		return -1;
	}
	// return the register value
	return _buffer[0];
}


/**MPU9250_Init
 *
 * @return
 */
int MPU9250_Init(){
	// select clock source to gyro
	if(writeRegister(PWR_MGMNT_1, CLOCK_SEL_PLL) < 0){
		return -1;
	}
	// enable I2C master mode
	if(writeRegister(USER_CTRL, I2C_MST_EN) < 0){
		return -2;
	}
	// set the I2C bus speed to 400 kHz
	if(writeRegister(I2C_MST_CTRL, I2C_MST_CLK) < 0){
		return -3;
	}
	// set AK8963 to Power Down
	writeAK8963Register(AK8963_CNTL1, AK8963_PWR_DOWN);
	// reset the MPU9250
	writeRegister(PWR_MGMNT_1, PWR_RESET);
	// wait for MPU-9250 to come back up
	delay(1);
	// reset the AK8963
	writeAK8963Register(AK8963_CNTL2, AK8963_RESET);
	// select clock source to gyro
	if(writeRegister(PWR_MGMNT_1, CLOCK_SEL_PLL) < 0){
		return -4;
	}
	// check the WHO AM I byte, expected value is 0x71 (decimal 113) or 0x73 (decimal 115)
	if((MPU9250_WhoAmI() != 113)&&(MPU9250_WhoAmI() != 115)){
		return -5;
	}
	// enable accelerometer and gyro
	if(writeRegister(PWR_MGMNT_2, SEN_ENABLE) < 0){
		return -6;
	}
	// setting accel range to 16G as default
	if(writeRegister(ACCEL_CONFIG, ACCEL_FS_SEL_16G) < 0){
		return -7;
	}
	_accelScale = GRAVITY * 16.0f/32767.5f; // setting the accel scale to 16G
	_accelRange = ACCEL_RANGE_16G;

	MPU9250_SetGyroRange(GYRO_RANGE_250DPS);

	MPU9250_SetDlpfBandwidth(DLPF_BANDWIDTH_184HZ);

	MPU9250_SetSrd(19);

	// enable I2C master mode
	if(writeRegister(USER_CTRL, I2C_MST_EN) < 0){
		return -12;
	}
	// set the I2C bus speed to 400 kHz
	if( writeRegister(I2C_MST_CTRL, I2C_MST_CLK) < 0){
		return -13;
	}
	// check AK8963 WHO AM I register, expected value is 0x48 (decimal 72)
	if( MPU9250_WhoAmIAK8963() != 72 ){
		return -14;
	}
	/* get the magnetometer calibration */
	// set AK8963 to Power Down
	if(writeAK8963Register(AK8963_CNTL1, AK8963_PWR_DOWN) < 0){
		return -15;
	}

	delay(100); // long wait between AK8963 mode changes

	// set AK8963 to FUSE ROM access
	if(writeAK8963Register(AK8963_CNTL1, AK8963_FUSE_ROM) < 0){
		return -16;
	}

	delay(100); // long wait between AK8963 mode changes
	// read the AK8963 ASA registers and compute magnetometer scale factors
	readAK8963Registers(AK8963_ASA, 3, _buffer);
	_magScaleX = ((((float)_buffer[0]) - 128.0f)/(256.0f) + 1.0f) * 4912.0f / 32760.0f; // micro Tesla
	_magScaleY = ((((float)_buffer[1]) - 128.0f)/(256.0f) + 1.0f) * 4912.0f / 32760.0f; // micro Tesla
	_magScaleZ = ((((float)_buffer[2]) - 128.0f)/(256.0f) + 1.0f) * 4912.0f / 32760.0f; // micro Tesla
	// set AK8963 to Power Down
	if(writeAK8963Register(AK8963_CNTL1, AK8963_PWR_DOWN) < 0){
		return -17;
	}

	delay(100); // long wait between AK8963 mode changes

	// set AK8963 to 16 bit resolution, 100 Hz update rate
	if(writeAK8963Register(AK8963_CNTL1, AK8963_CNT_MEAS2) < 0){
		return -18;
	}

	delay(100); // long wait between AK8963 mode changes

	// select clock source to gyro
	if(writeRegister(PWR_MGMNT_1, CLOCK_SEL_PLL) < 0){
		return -19;
	}
	// instruct the MPU9250 to get 7 bytes of data from the AK8963 at the sample rate
	readAK8963Registers(AK8963_HXL, 7, _buffer);

	MPU9250_Calibrate();
	/*if (calibrateGyro() < 0) {
		return -20;
	}*/
	// successful init, return 1
	return 1;
}


/**MPU9250_ReadSensor
 * reads the most current data from MPU9250 and stores in buffer
 * @return
 */
int MPU9250_ReadSensor() {
	// read the data from the MPU9250
	if (readRegisters(ACCEL_OUT, 14, _buffer) < 0) {
		return -1;
	}
	// combine into 16 bit values
	raw_accel_x = (((int16_t)_buffer[0]) << 8) | _buffer[1];
	raw_accel_y = (((int16_t)_buffer[2]) << 8) | _buffer[3];
	raw_accel_z = (((int16_t)_buffer[4]) << 8) | _buffer[5];
	raw_temperature = (((int16_t)_buffer[6]) << 8) | _buffer[7];
	raw_gyro_x = (((int16_t)_buffer[8]) << 8) | _buffer[9];
	raw_gyro_y = (((int16_t)_buffer[10]) << 8) | _buffer[11];
	raw_gyro_z = (((int16_t)_buffer[12]) << 8) | _buffer[13];

#if 0
	// read the data from the AK8963
	if (readAK8963Registers(AK8963_HXL, 7, &_buffer[14]) < 0) {
		return -1;
	}
	raw_magneto_x = (((int16_t)_buffer[15]) << 8) | _buffer[14];
	raw_magneto_y = (((int16_t)_buffer[17]) << 8) | _buffer[16];
	raw_magneto_z = (((int16_t)_buffer[19]) << 8) | _buffer[18];

	magneto_x = (((float)(raw_magneto_x) * _magScaleX) - _hxb)*_hxs;
	magneto_y = (((float)(raw_magneto_y) * _magScaleY) - _hyb)*_hys;
	magneto_z = (((float)(raw_magneto_z) * _magScaleZ) - _hzb)*_hzs;
#endif
	tempera = ((((float) raw_temperature) - _tempOffset)/_tempScale) + _tempOffset;
	return 1;

}

/**MPU9250_Calibrate
 *
 */
void MPU9250_Calibrate(){
	int calibration_sample_count = 100;
	float x_accel = 0;
	float y_accel = 0;
	float z_accel = 0;
	float x_gyro = 0;
	float y_gyro = 0;
	float z_gyro = 0;

	for (int i=0; i < calibration_sample_count; i++) {
		MPU9250_ReadSensor();
		x_accel += raw_accel_x;
		y_accel += raw_accel_y;
		z_accel += raw_accel_z;
		x_gyro += raw_gyro_x;
		y_gyro += raw_gyro_y;
		z_gyro += raw_gyro_z;
		delay(20);
	}

	x_accel /= calibration_sample_count;
	y_accel /= calibration_sample_count;
	z_accel /= calibration_sample_count;
	x_gyro /= calibration_sample_count;
	y_gyro /= calibration_sample_count;
	z_gyro /= calibration_sample_count;

	// Store the raw calibration values globally
	base_x_accel = x_accel;
	base_y_accel = y_accel;
	base_z_accel = z_accel;
	base_x_gyro = x_gyro;
	base_y_gyro = y_gyro;
	base_z_gyro = z_gyro;
}

/**MPU9250_SetGyroRange
 * sets the gyro full scale range to values other than default
 * @param range
 * @return
 */
int MPU9250_SetGyroRange(mpu_gyroscope_range_t range) {
	switch(range) {
	case GYRO_RANGE_250DPS: {
		// setting the gyro range to 250DPS
		if(writeRegister(GYRO_CONFIG,GYRO_FS_SEL_250DPS) < 0){
			return -1;
		}
		_gyroScale = 250.0f/32767.5f * DEGREE_TO_RADIAN; // setting the gyro scale to 250DPS
		break;
	}
	case GYRO_RANGE_500DPS: {
		// setting the gyro range to 500DPS
		if(writeRegister(GYRO_CONFIG,GYRO_FS_SEL_500DPS) < 0){
			return -1;
		}
		_gyroScale = 500.0f/32767.5f * DEGREE_TO_RADIAN; // setting the gyro scale to 500DPS
		break;
	}
	case GYRO_RANGE_1000DPS: {
		// setting the gyro range to 1000DPS
		if(writeRegister(GYRO_CONFIG,GYRO_FS_SEL_1000DPS) < 0){
			return -1;
		}
		_gyroScale = 1000.0f/32767.5f * DEGREE_TO_RADIAN; // setting the gyro scale to 1000DPS
		break;
	}
	case GYRO_RANGE_2000DPS: {
		// setting the gyro range to 2000DPS
		if(writeRegister(GYRO_CONFIG,GYRO_FS_SEL_2000DPS) < 0){
			return -1;
		}
		_gyroScale = 2000.0f/32767.5f * DEGREE_TO_RADIAN; // setting the gyro scale to 2000DPS
		break;
	}
	}
	_gyroRange = range;
	return 1;
}

/**MPU9250_SetDlpfBandwidth
 * sets the DLPF bandwidth to values other than default
 * @param bandwidth
 * @return
 */
int MPU9250_SetDlpfBandwidth(mpu_dlpf_bandwidth_t bandwidth) {
	switch(bandwidth) {
	case DLPF_BANDWIDTH_184HZ: {
		if(writeRegister(ACCEL_CONFIG2,ACCEL_DLPF_184) < 0){ // setting accel bandwidth to 184Hz
			return -1;
		}
		if(writeRegister(CONFIG,GYRO_DLPF_184) < 0){ // setting gyro bandwidth to 184Hz
			return -2;
		}
		break;
	}
	case DLPF_BANDWIDTH_92HZ: {
		if(writeRegister(ACCEL_CONFIG2,ACCEL_DLPF_92) < 0){ // setting accel bandwidth to 92Hz
			return -1;
		}
		if(writeRegister(CONFIG,GYRO_DLPF_92) < 0){ // setting gyro bandwidth to 92Hz
			return -2;
		}
		break;
	}
	case DLPF_BANDWIDTH_41HZ: {
		if(writeRegister(ACCEL_CONFIG2,ACCEL_DLPF_41) < 0){ // setting accel bandwidth to 41Hz
			return -1;
		}
		if(writeRegister(CONFIG,GYRO_DLPF_41) < 0){ // setting gyro bandwidth to 41Hz
			return -2;
		}
		break;
	}
	case DLPF_BANDWIDTH_20HZ: {
		if(writeRegister(ACCEL_CONFIG2,ACCEL_DLPF_20) < 0){ // setting accel bandwidth to 20Hz
			return -1;
		}
		if(writeRegister(CONFIG,GYRO_DLPF_20) < 0){ // setting gyro bandwidth to 20Hz
			return -2;
		}
		break;
	}
	case DLPF_BANDWIDTH_10HZ: {
		if(writeRegister(ACCEL_CONFIG2,ACCEL_DLPF_10) < 0){ // setting accel bandwidth to 10Hz
			return -1;
		}
		if(writeRegister(CONFIG,GYRO_DLPF_10) < 0){ // setting gyro bandwidth to 10Hz
			return -2;
		}
		break;
	}
	case DLPF_BANDWIDTH_5HZ: {
		if(writeRegister(ACCEL_CONFIG2,ACCEL_DLPF_5) < 0){ // setting accel bandwidth to 5Hz
			return -1;
		}
		if(writeRegister(CONFIG,GYRO_DLPF_5) < 0){ // setting gyro bandwidth to 5Hz
			return -2;
		}
		break;
	}
	}
	_bandwidth = bandwidth;
	return 1;
}

/**MPU9250_SetSrd
 * sets the sample rate divider to values other than default
 * @param srd
 * @return
 */
int MPU9250_SetSrd(uint8_t srd) {
	/* setting the sample rate divider to 19 to facilitate setting up magnetometer */
	if(writeRegister(SMPDIV,19) < 0){ // setting the sample rate divider
		return -1;
	}
	if(srd > 9){
		// set AK8963 to Power Down
		if(writeAK8963Register(AK8963_CNTL1,AK8963_PWR_DOWN) < 0){
			return -2;
		}
		delay(100); // long wait between AK8963 mode changes
		// set AK8963 to 16 bit resolution, 8 Hz update rate
		if(writeAK8963Register(AK8963_CNTL1,AK8963_CNT_MEAS1) < 0){
			return -3;
		}
		delay(100); // long wait between AK8963 mode changes
		// instruct the MPU9250 to get 7 bytes of data from the AK8963 at the sample rate
		readAK8963Registers(AK8963_HXL,7,_buffer);
	} else {
		// set AK8963 to Power Down
		if(writeAK8963Register(AK8963_CNTL1,AK8963_PWR_DOWN) < 0){
			return -2;
		}
		delay(100); // long wait between AK8963 mode changes
		// set AK8963 to 16 bit resolution, 100 Hz update rate
		if(writeAK8963Register(AK8963_CNTL1,AK8963_CNT_MEAS2) < 0){
			return -3;
		}
		delay(100); // long wait between AK8963 mode changes
		// instruct the MPU9250 to get 7 bytes of data from the AK8963 at the sample rate
		readAK8963Registers(AK8963_HXL,7,_buffer);
	}
	/* setting the sample rate divider */
	if(writeRegister(SMPDIV,srd) < 0){ // setting the sample rate divider
		return -4;
	}
	_srd = srd;
	return 1;
}


/**ComplementaryFilter
 *
 * @param pitch
 * @param roll
 */
void ComplementaryFilter(float *pitch, float *roll, uint32_t deltaT){

	angularVelocityX = (float)(raw_gyro_x - base_x_gyro) / 131.0f;
	angularVelocityY = (float)(raw_gyro_y - base_y_gyro) / 131.0f;

	aRoll = atan((float)raw_accel_y/sqrt(pow((float)raw_accel_x,2) + pow((float)raw_accel_z,2)));
	aPitch = atan(-1*(float)raw_accel_x/sqrt(pow((float)raw_accel_y,2) + pow((float)raw_accel_z,2)));

	dt = (float)deltaT/1000.0f;
	alpha = 0.95;
	*roll = alpha * (angularVelocityX * dt + oldRoll) + (1 - alpha) * aRoll * RADIANS_TO_DEGREES;
	*pitch = alpha * (angularVelocityY * dt + aPitch) + (1 - alpha) * aPitch * RADIANS_TO_DEGREES;

	oldRoll = *roll;
	oldPitch = *pitch;
}
