#include "MPU6050.h"
#include <string.h>

extern I2C_HandleTypeDef hi2c1;

int16_t Gyro_X_RAW, Gyro_Y_RAW, Gyro_Z_RAW;
float Gx,Gy,Gz;
int16_t Accel_X_RAW, Accel_Y_RAW, Accel_Z_RAW;
float Ax,Ay,Az;

void MPU6050_Init (void)
{
	uint8_t check = 0;
	//function reads data from specific register (memory address)
	//HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
	HAL_I2C_Mem_Read(
			&hi2c1, //I2C_HandleTypeDef pointer
			MPU6050_ADDR, //the I²C slave address of your device  				  --I2C_HandleTypeDef *hi2c--
			WHO_AM_I_REG,         //memory address						 				  --uint16_t DevAddress--
			1,			  //Tells HAL how many bytes wide the register address is --uint16_t MemAddress--
			&check,		  // pointer to where to store data						  --uint8_t *pData--
			1,			  //number of bytes to read from device					  --uint16_t Size--
			1000		  //timeout in ms										  --uint32_t Timeout--
	);
	if (check == 0x68)
	{
		// power management register 0X6B we should write all 0's to wake the sensor up
		/*  DEVICE_RESET When set to 1, this bit resets all internal registers to their default values.
				 		 The bit automatically clears to 0 once the reset is done.
				 		 The default values for each register can be found in Section 3.
			SLEEP When set to 1, this bit puts the MPU-60X0 into sleep mode.
			CYCLE When this bit is set to 1 and SLEEP is disabled, the MPU-60X0 will cycle
				  between sleep mode and waking up to take a single sample of data from
				  active sensors at a rate determined by LP_WAKE_CTRL (register 108).
			TEMP_DIS When set to 1, this bit disables the temperature sensor.
			CLKSEL 3-bit unsigned value. Specifies the clock source of the device. */
	    uint8_t Data = 0x00;
		//HAL_I2C_Mem_Write_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size)
		HAL_I2C_Mem_Write(
				&hi2c1,
				MPU6050_ADDR,
				PWR_MGMT_1_REG,
				1,
				&Data,
				1,
				1000
		);

		//sample rate 8khz
		Data = 0x00;
		HAL_I2C_Mem_Write(
				&hi2c1,
				MPU6050_ADDR,
				SMPLRT_DIV_REG,
				1,
				&Data,
				1,
				1000
		);
		//Gyroscope Configuration no self tests +/- 250 degrees/s
		Data = 0x00;
		HAL_I2C_Mem_Write(
				&hi2c1,
				MPU6050_ADDR,
				GYRO_CONFIG_REG,
				1,
				&Data,
				1,
				1000
		);
		//Accelerometer Configuration no self tests +/- 2g
		Data = 0x00;
		HAL_I2C_Mem_Write(
				&hi2c1,
				MPU6050_ADDR,
				ACCEL_CONFIG_REG,
				1,
				&Data,
				1,
				1000
		);
	}
}
void MPU6050_Read_Accel (void)
{
	uint8_t Rec_Data[6];
	HAL_I2C_Mem_Read(
				&hi2c1,
				MPU6050_ADDR, //the I²C slave address of your device  				  --I2C_HandleTypeDef *hi2c--
				ACCEL_XOUT_H,         //memory address						 				  --uint16_t DevAddress--
				1,			  //Tells HAL how many bytes wide the register address is --uint16_t MemAddress--
				Rec_Data,		  // pointer to where to store data						  --uint8_t *pData--
				6,			  //number of bytes to read from device					  --uint16_t Size--
				1000
	);

	Accel_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data [1]);
	Accel_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data [3]);
	Accel_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data [5]);

	Ax = (float)Accel_X_RAW/16384.0;
	Ay = (float)Accel_Y_RAW/16384.0;
	Az = (float)Accel_Z_RAW/16384.0;
}
void MPU6050_Read_Gyro (void)

{
	uint8_t Rec_Data[6];
	HAL_I2C_Mem_Read(
				&hi2c1,
				MPU6050_ADDR, //the I²C slave address of your device  				  --I2C_HandleTypeDef *hi2c--
				GYRO_XOUT_H,         //memory address						 				  --uint16_t DevAddress--
				1,			  //Tells HAL how many bytes wide the register address is --uint16_t MemAddress--
				Rec_Data,		  // pointer to where to store data						  --uint8_t *pData--
				6,			  //number of bytes to read from device					  --uint16_t Size--
				1000
	);

	Gyro_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data [1]);
	Gyro_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data [3]);
	Gyro_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data [5]);

	Gx = (float)Gyro_X_RAW/131.0;
	Gy = (float)Gyro_Y_RAW/131.0;
	Gz = (float)Gyro_Z_RAW/131.0;
}
