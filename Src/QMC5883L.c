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
    1,
    1000
  );

  if (status != HAL_OK){
    *error = QMC5883L_I2C_ERROR;
    return status;
  }
  
  if (Chip_ID != 0xFF){
    *error = QMC5883L_CHIP_ID_ERROR;
    return HAL_ERROR;
  }

  // This is can be changed depending on the settings needed
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
    return status;
  }

  data = 0x00;
  status = HAL_I2C_Mem_Write(
    hi2c,
    QMC_ADDRESS,
    QMC_REG_CR2,
    I2C_MEMADD_SIZE_8BIT,
    &data,
    1,
    1000
  );

  if (status != HAL_OK) {
    *error = QMC5883L_CONFIG2_ERROR;
    return status;
  }

  data = 0x01;
  status = HAL_I2C_Mem_Write(
    hi2c,
    QMC_ADDRESS,
    QMC_REG_SR,
    I2C_MEMADD_SIZE_8BIT,
    &data,
    1,
    1000
  );

  if (status != HAL_OK) {
    *error = QMC5883L_SR_ERROR;
    return status;
  }
  return HAL_OK;
}

// Saves raw data
HAL_StatusTypeDef QMC_ReadGauss(I2C_HandleTypeDef *hi2c, QMC5883L_ERROR *error, QMC5883L_Data_t *data){
  uint8_t status_reg = 0;
  HAL_StatusTypeDef status = HAL_I2C_Mem_Read(
    hi2c,
    QMC_ADDRESS,
    QMC_REG_STATUS,
    I2C_MEMADD_SIZE_8BIT,
    &status_reg,
    1,
    1000
  );
  
  if (status != HAL_OK){
    *error = QMC5883L_STATUS_ERROR;
    return status;
  }
  
  if ((status_reg & 0x01) == 0){
    return HAL_OK;
  }

    int8_t data_raw[6];
    status = HAL_I2C_Mem_Read(
      hi2c,
      QMC_ADDRESS,
      QMC_REG_OUT_X_M,
      I2C_MEMADD_SIZE_8BIT,
      data_raw,
      6,
      1000
    );

    if (status != HAL_OK){
      *error = QMC5883L_DATAREAD_ERROR;
      return status;
    }

    data -> Mx = (int16_t)((data_raw[1] << 8)| data_raw[0]); 
    data -> My = (int16_t)((data_raw[3] << 8)| data_raw[2]);  
    data -> Mz = (int16_t)((data_raw[5] << 8)| data_raw[4]); 


  return HAL_OK;
}