#ifndef QMC5883L_H
#define QMC5883L_H

#include <stdint.h>
#include "stm32l4xx_hal.h"

extern I2C_HandleTypeDef hi2c1;

#define QMC_ADDRESS      (0x0D << 1)
#define QMC_REG_CR1      0x09
#define QMC_REG_CR2      0x0A
#define QMC_REG_SR		 0x0B
#define QMC_REG_OUT_X_M  0x00
#define QMC_REG_ID       0x0D


extern float Mx, My, Mz; // in gauss (scaled)

HAL_StatusTypeDef QMC_Init(void);
HAL_StatusTypeDef QMC_ReadGauss(void);

#endif
