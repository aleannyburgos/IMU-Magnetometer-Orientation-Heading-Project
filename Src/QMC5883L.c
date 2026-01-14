#include "QMC5883L.h"
#include <stdint.h>
#include "stm32l4xx_hal.h"

// here we set the config registers and mode
HAL_StatusTypeDef QMC_Init(I2C_HandleTypeDef *hi2c, QMC5883L_ERROR *error){
  uint8_t Chip_ID = 0;
  HAL_StatusTypeDef status = HAL_I2C_Mem_Read(
    hi2c,
    QMC_ADDRESS,
    QMC_REG_ID,
    I2C_MEMADD_SIZE_8BIT,
    &Chip_ID,
    1
  );

  if (status != HAL_OK){
    *error = QMC5883L_I2C_ERROR;
    return error;
  }
  
  if (Chip_ID != 0xFF){
    *error = QMC5883L_CHIP_ID_ERROR;
    return error;
  }

  uint8_t data = (OSR_512 | RNG_2G | ODR_10HZ | MODE_CONTIN);
  status = HAL_I2C_Mem_Write(
    hi2c,
    QMC_ADDRESS,
    QMC_REG_CR1,
    I2C_MEMADD_SIZE_8BIT,
    &data,
    1,
    1000
  );

  if (status != HAL_OK) {
    *error = QMC5883L_CONFIG1_ERROR;
    return error;
  }

  return HAL_OK;
}

HAL_StatusTypeDef QMC_ReadGauss(I2C_HandleTypeDef *hi2c, QMC5883L_ERROR *error, QMC5883L_Data_t *data){
  uint8_t data = 0;
  HAL_StatusTypeDef status = HAL_I2C_Mem_Read(
    hi2c,
    QMC_ADDRESS,
    QMC_REG_OUT_X_M,
    I2C_MEMADD_SIZE_8BIT,
    &data,
    1
  );
  
  if (status != HAL_OK){
    *error = QMC5883L_STATUS_ERROR;
    return error;
  }
  
  if (data == (1 || 3 || 5)) {
    int8_t data_r [6];
    status = HAL_I2C_Mem_Read(
      hi2c,
      QMC_ADDRESS,
      QMC_REG_STATUS,
      I2C_MEMADD_SIZE_8BIT,
      &data_r,
      6
    );

    if (status != HAL_OK){
      *error = QMC5883L_DATAREAD_ERROR;
      return error;
    }

    data -> Mx = (int16_t)((data_r[1] << 8)| data_r[0]); 
    data -> My = (int16_t)((data_r[3] << 8)| data_r[2]);  
    data -> Mz = (int16_t)((data_r[5] << 8)| data_r[4]); 

  };

  return HAL_OK;
}