/*
 * mpu9250.c
 *
 *  Created on: 10 Oca 2021
 *      Author: Azad
 */

#include "mpu9250.h"
#include "stdlib.h"

// track success of interacting with sensor
int _status;

// buffer for reading from sensor
uint8_t _buffer[21];

// data counts
int16_t _axcounts,_aycounts,_azcounts;
int16_t _gxcounts,_gycounts,_gzcounts;
int16_t _hxcounts,_hycounts,_hzcounts;
int16_t _tcounts;

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

// transformation matrix
/* transform the accel and gyro axes to match the magnetometer axes */
const int16_t tX[3] = {0,  1,  0};
const int16_t tY[3] = {1,  0,  0};
const int16_t tZ[3] = {0,  0, -1};

// constants
const float G = 9.807f;
const float _d2r = 3.14159265359f/180.0f;



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

/**
 *
 * @param subAddress
 * @param count
 * @param dest
 * @return
 */
static int readRegisters(uint8_t subAddress, uint8_t count, uint8_t* dest){
	HAL_I2C_Master_Transmit(&MPU9250_I2C, MPU_I2C_ADDRESS, &subAddress, 1, 0xFF);
	HAL_I2C_Master_Receive(&MPU9250_I2C, MPU_I2C_ADDRESS, dest, count, 0xFF);
	return 1;
}

/**
 *
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

/**
 *
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
	// read the bytes off the MPU9250 EXT_SENS_DATA registers
	_status = readRegisters(EXT_SENS_DATA_00, count, dest);
	return _status;
}


/**whoAmI
 *
 * @return
 */
int whoAmI(){
	// read the WHO AM I register
	if (readRegisters(WHO_AM_I,1,_buffer) < 0) {
		return -1;
	}
	// return the register value
	return _buffer[0];
}


/**whoAmIAK8963
 *
 * @return
 */
int whoAmIAK8963(){
	// read the WHO AM I register
	if (readAK8963Registers(AK8963_WHO_AM_I,1,_buffer) < 0) {
		return -1;
	}
	// return the register value
	return _buffer[0];
}


/**mpu_init
 *
 * @return
 */
int mpu_init(){
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
	if((whoAmI() != 113)&&(whoAmI() != 115)){
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
	_accelScale = G * 16.0f/32767.5f; // setting the accel scale to 16G
	_accelRange = ACCEL_RANGE_16G;
	// setting the gyro range to 2000DPS as default
	if(writeRegister(GYRO_CONFIG, GYRO_FS_SEL_2000DPS) < 0){
		return -8;
	}
	_gyroScale = 2000.0f/32767.5f * _d2r; // setting the gyro scale to 2000DPS
	_gyroRange = GYRO_RANGE_2000DPS;
	// setting bandwidth to 184Hz as default
	if(writeRegister(ACCEL_CONFIG2, ACCEL_DLPF_184) < 0){
		return -9;
	}
	if(writeRegister(CONFIG, GYRO_DLPF_184) < 0){ // setting gyro bandwidth to 184Hz
		return -10;
	}
	_bandwidth = DLPF_BANDWIDTH_184HZ;
	// setting the sample rate divider to 0 as default
	if(writeRegister(SMPDIV, 0x00) < 0){
		return -11;
	}
	_srd = 0;
	// enable I2C master mode
	if(writeRegister(USER_CTRL, I2C_MST_EN) < 0){
		return -12;
	}
	// set the I2C bus speed to 400 kHz
	if( writeRegister(I2C_MST_CTRL, I2C_MST_CLK) < 0){
		return -13;
	}
	// check AK8963 WHO AM I register, expected value is 0x48 (decimal 72)
	if( whoAmIAK8963() != 72 ){
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
	// estimate gyro bias
	if (calibrateGyro() < 0) {
		return -20;
	}
	// successful init, return 1
	return 1;
}


/**readSensor
 * reads the most current data from MPU9250 and stores in buffer
 * @return
 */
int readSensor() {
	// read the data from the MPU9250
	if (readRegisters(ACCEL_OUT, 14, _buffer) < 0) {
		return -1;
	}
	// combine into 16 bit values
	_axcounts = (((int16_t)_buffer[0]) << 8) | _buffer[1];
	_aycounts = (((int16_t)_buffer[2]) << 8) | _buffer[3];
	_azcounts = (((int16_t)_buffer[4]) << 8) | _buffer[5];
	_tcounts = (((int16_t)_buffer[6]) << 8) | _buffer[7];
	_gxcounts = (((int16_t)_buffer[8]) << 8) | _buffer[9];
	_gycounts = (((int16_t)_buffer[10]) << 8) | _buffer[11];
	_gzcounts = (((int16_t)_buffer[12]) << 8) | _buffer[13];

	// read the data from the AK8963
	if (readAK8963Registers(AK8963_HXL, 7, &_buffer[14]) < 0) {
			return -1;
	}
	_hxcounts = (((int16_t)_buffer[15]) << 8) | _buffer[14];
	_hycounts = (((int16_t)_buffer[17]) << 8) | _buffer[16];
	_hzcounts = (((int16_t)_buffer[19]) << 8) | _buffer[18];

	// transform and convert to float values
	accel_x = (((float)(tX[0]*_axcounts + tX[1]*_aycounts + tX[2]*_azcounts) * _accelScale) - _axb)*_axs;
	accel_y = (((float)(tY[0]*_axcounts + tY[1]*_aycounts + tY[2]*_azcounts) * _accelScale) - _ayb)*_ays;
	accel_z = (((float)(tZ[0]*_axcounts + tZ[1]*_aycounts + tZ[2]*_azcounts) * _accelScale) - _azb)*_azs;
	gyro_x = ((float)(tX[0]*_gxcounts + tX[1]*_gycounts + tX[2]*_gzcounts) * _gyroScale) - _gxb;
	gyro_y = ((float)(tY[0]*_gxcounts + tY[1]*_gycounts + tY[2]*_gzcounts) * _gyroScale) - _gyb;
	gyro_z = ((float)(tZ[0]*_gxcounts + tZ[1]*_gycounts + tZ[2]*_gzcounts) * _gyroScale) - _gzb;

	magneto_x = (((float)(_hxcounts) * _magScaleX) - _hxb)*_hxs;
	magneto_y = (((float)(_hycounts) * _magScaleY) - _hyb)*_hys;
	magneto_z = (((float)(_hzcounts) * _magScaleZ) - _hzb)*_hzs;

	tempera = ((((float) _tcounts) - _tempOffset)/_tempScale) + _tempOffset;
	return 1;

}


/**calibrateGyro
 * estimates the gyro biases
 * @return
 */
int calibrateGyro() {
	// set the range, bandwidth, and srd
	if (setGyroRange(GYRO_RANGE_250DPS) < 0) {
		return -1;
	}
	if (setDlpfBandwidth(DLPF_BANDWIDTH_20HZ) < 0) {
		return -2;
	}
	if (setSrd(19) < 0) {
		return -3;
	}

	// take samples and find bias
	_gxbD = 0;
	_gybD = 0;
	_gzbD = 0;
	for (size_t i=0; i < _numSamples; i++) {
		readSensor();
		_gxbD += (getGyroX_rads() + _gxb)/((double)_numSamples);
		_gybD += (getGyroY_rads() + _gyb)/((double)_numSamples);
		_gzbD += (getGyroZ_rads() + _gzb)/((double)_numSamples);
		delay(20);
	}
	_gxb = (float)_gxbD;
	_gyb = (float)_gybD;
	_gzb = (float)_gzbD;

	// set the range, bandwidth, and srd back to what they were
	if (setGyroRange(_gyroRange) < 0) {
		return -4;
	}
	if (setDlpfBandwidth(_bandwidth) < 0) {
		return -5;
	}
	if (setSrd(_srd) < 0) {
		return -6;
	}
	return 1;
}
/* finds bias and scale factor calibration for the magnetometer,
the sensor should be rotated in a figure 8 motion until complete */
int calibrateMagneto(void) {
	// set the srd
	if (setSrd(19) < 0) {
		return -1;
	}

	// get a starting set of data
	readSensor();
	_hxmax = getMagX_uT();
	_hxmin = getMagX_uT();
	_hymax = getMagY_uT();
	_hymin = getMagY_uT();
	_hzmax = getMagZ_uT();
	_hzmin = getMagZ_uT();

	// collect data to find max / min in each channel
	_counter = 0;
	while (_counter < _maxCounts) {
		_delta = 0.0f;
		_framedelta = 0.0f;
		readSensor();
		_hxfilt = (_hxfilt*((float)_coeff-1)+(getMagX_uT()/_hxs+_hxb))/((float)_coeff);
		_hyfilt = (_hyfilt*((float)_coeff-1)+(getMagY_uT()/_hys+_hyb))/((float)_coeff);
		_hzfilt = (_hzfilt*((float)_coeff-1)+(getMagZ_uT()/_hzs+_hzb))/((float)_coeff);
		if (_hxfilt > _hxmax) {
			_delta = _hxfilt - _hxmax;
			_hxmax = _hxfilt;
		}
		if (_delta > _framedelta) {
			_framedelta = _delta;
		}
		if (_hyfilt > _hymax) {
			_delta = _hyfilt - _hymax;
			_hymax = _hyfilt;
		}
		if (_delta > _framedelta) {
			_framedelta = _delta;
		}
		if (_hzfilt > _hzmax) {
			_delta = _hzfilt - _hzmax;
			_hzmax = _hzfilt;
		}
		if (_delta > _framedelta) {
			_framedelta = _delta;
		}
		if (_hxfilt < _hxmin) {
			_delta = abs(_hxfilt - _hxmin);
			_hxmin = _hxfilt;
		}
		if (_delta > _framedelta) {
			_framedelta = _delta;
		}
		if (_hyfilt < _hymin) {
			_delta = abs(_hyfilt - _hymin);
			_hymin = _hyfilt;
		}
		if (_delta > _framedelta) {
			_framedelta = _delta;
		}
		if (_hzfilt < _hzmin) {
			_delta = abs(_hzfilt - _hzmin);
			_hzmin = _hzfilt;
		}
		if (_delta > _framedelta) {
			_framedelta = _delta;
		}
		if (_framedelta > _deltaThresh) {
			_counter = 0;
		} else {
			_counter++;
		}
		delay(20);
	}

	// find the magnetometer bias
	_hxb = (_hxmax + _hxmin) / 2.0f;
	_hyb = (_hymax + _hymin) / 2.0f;
	_hzb = (_hzmax + _hzmin) / 2.0f;

	// find the magnetometer scale factor
	_hxs = (_hxmax - _hxmin) / 2.0f;
	_hys = (_hymax - _hymin) / 2.0f;
	_hzs = (_hzmax - _hzmin) / 2.0f;
	_avgs = (_hxs + _hys + _hzs) / 3.0f;
	_hxs = _avgs/_hxs;
	_hys = _avgs/_hys;
	_hzs = _avgs/_hzs;

	// set the srd back to what it was
	if (setSrd(_srd) < 0) {
		return -2;
	}
	return 1;
}

/* returns the gyro bias in the X direction, rad/s */
float getGyroBiasX_rads() {
	return _gxb;
}

/* returns the gyro bias in the Y direction, rad/s */
float getGyroBiasY_rads() {
	return _gyb;
}

/* returns the gyro bias in the Z direction, rad/s */
float getGyroBiasZ_rads() {
	return _gzb;
}

/* sets the gyro bias in the X direction to bias, rad/s */
void setGyroBiasX_rads(float bias) {
	_gxb = bias;
}

/* sets the gyro bias in the Y direction to bias, rad/s */
void setGyroBiasY_rads(float bias) {
	_gyb = bias;
}

/* sets the gyro bias in the Z direction to bias, rad/s */
void setGyroBiasZ_rads(float bias) {
	_gzb = bias;
}




/* returns the accelerometer measurement in the x direction, m/s/s */
float getAccelX_mss() {
	return accel_x;
}

/* returns the accelerometer measurement in the y direction, m/s/s */
float getAccelY_mss() {
	return accel_y;
}

/* returns the accelerometer measurement in the z direction, m/s/s */
float getAccelZ_mss() {
	return accel_z;
}

/* returns the gyroscope measurement in the x direction, rad/s */
float getGyroX_rads() {
	return gyro_x;
}

/* returns the gyroscope measurement in the y direction, rad/s */
float getGyroY_rads() {
	return gyro_y;
}

/* returns the gyroscope measurement in the z direction, rad/s */
float getGyroZ_rads() {
	return gyro_z;
}

/* returns the magnetometer measurement in the x direction, uT */
float getMagX_uT() {
	return magneto_x;
}

/* returns the magnetometer measurement in the y direction, uT */
float getMagY_uT() {
	return magneto_y;
}

/* returns the magnetometer measurement in the z direction, uT */
float getMagZ_uT() {
	return magneto_z;
}

/* returns the die temperature, C */
float getTemperature_C() {
	return tempera;
}

/**setGyroRange
 * sets the gyro full scale range to values other than default
 * @param range
 * @return
 */
int setGyroRange(mpu_gyroscope_range_t range) {
	switch(range) {
	case GYRO_RANGE_250DPS: {
		// setting the gyro range to 250DPS
		if(writeRegister(GYRO_CONFIG,GYRO_FS_SEL_250DPS) < 0){
			return -1;
		}
		_gyroScale = 250.0f/32767.5f * _d2r; // setting the gyro scale to 250DPS
		break;
	}
	case GYRO_RANGE_500DPS: {
		// setting the gyro range to 500DPS
		if(writeRegister(GYRO_CONFIG,GYRO_FS_SEL_500DPS) < 0){
			return -1;
		}
		_gyroScale = 500.0f/32767.5f * _d2r; // setting the gyro scale to 500DPS
		break;
	}
	case GYRO_RANGE_1000DPS: {
		// setting the gyro range to 1000DPS
		if(writeRegister(GYRO_CONFIG,GYRO_FS_SEL_1000DPS) < 0){
			return -1;
		}
		_gyroScale = 1000.0f/32767.5f * _d2r; // setting the gyro scale to 1000DPS
		break;
	}
	case GYRO_RANGE_2000DPS: {
		// setting the gyro range to 2000DPS
		if(writeRegister(GYRO_CONFIG,GYRO_FS_SEL_2000DPS) < 0){
			return -1;
		}
		_gyroScale = 2000.0f/32767.5f * _d2r; // setting the gyro scale to 2000DPS
		break;
	}
	}
	_gyroRange = range;
	return 1;
}

/**setDlpfBandwidth
 * sets the DLPF bandwidth to values other than default
 * @param bandwidth
 * @return
 */
int setDlpfBandwidth(mpu_dlpf_bandwidth_t bandwidth) {
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

/**setSrd
 * sets the sample rate divider to values other than default
 * @param srd
 * @return
 */
int setSrd(uint8_t srd) {
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

