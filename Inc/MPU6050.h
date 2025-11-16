#ifndef MPU6050_H
#define MPU6050_H

#include <stdint.h>
#include "stm32l4xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MPU6050_ADDR (0x68 << 1)//this is AD0 = GND
#define WHO_AM_I_REG 0x75
#define PWR_MGMT_1_REG 0x6B
#define SMPLRT_DIV_REG 0x19
#define GYRO_CONFIG_REG 0x1B
#define ACCEL_CONFIG_REG 0x1C
#define ACCEL_XOUT_H 0x3B
#define GYRO_XOUT_H  0x43

extern int16_t Gyro_X_RAW, Gyro_Y_RAW, Gyro_Z_RAW;
extern int16_t Accel_X_RAW, Accel_Y_RAW, Accel_Z_RAW;
extern float Gx, Gy, Gz; // deg/s
extern float Ax, Ay, Az; // g

void MPU6050_Init(void);
void MPU6050_Read_Accel(void);
void MPU6050_Read_Gyro(void);

#ifdef __cplusplus
}
#endif

#endif