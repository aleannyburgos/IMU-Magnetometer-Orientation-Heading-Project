#include "QMC5883L.h"
float Mx, My, Mz;
#include <string.h>


//returns status of write/read operation

static HAL_StatusTypeDef QMC_write(uint8_t reg, uint8_t v){
  return HAL_I2C_Mem_Write(&hi2c1, QMC_ADDRESS, reg, I2C_MEMADD_SIZE_8BIT, &v, 1, 100);
}
static HAL_StatusTypeDef QMC_read(uint8_t reg, uint8_t *buf, uint16_t n){
  return HAL_I2C_Mem_Read(&hi2c1, QMC_ADDRESS, reg, I2C_MEMADD_SIZE_8BIT, buf, n, 100);
}

// here we set the config registers and mode
HAL_StatusTypeDef QMC_Init(void){
/*  uint8_t id;
  if (QMC_read(QMC_REG_ID, &id, 1) != HAL_OK){
	  return HAL_ERROR;
  }
 */

  //for setting the data output rate and measurement configuration. These are set to default.
  if (QMC_write(QMC_REG_CR1,  0x45) != HAL_OK) return HAL_ERROR;// configure: 10Hz 64Hz sample rate, C1
  if (QMC_write(QMC_REG_CR2,  0x00) != HAL_OK) return HAL_ERROR;// reset and interrupt
  if (QMC_write(QMC_REG_SR, 0x01) != HAL_OK) return HAL_ERROR;//recommended
  return HAL_OK;
}

HAL_StatusTypeDef QMC_ReadGauss(void){
  uint8_t b[6];
  if (QMC_read(QMC_REG_OUT_X_M, b, 6) != HAL_OK) return HAL_ERROR;
  int16_t x = (int16_t)((b[1]<<8) | b[0]);
  int16_t y = (int16_t)((b[3]<<8) | b[2]);
  int16_t z = (int16_t)((b[5]<<8) | b[4]);
  const float lsb_per_gauss = 12000.0f; // for CRB=0x20
  Mx = x / lsb_per_gauss;
  My = y / lsb_per_gauss;
  Mz = z / lsb_per_gauss;
  return HAL_OK;
}
