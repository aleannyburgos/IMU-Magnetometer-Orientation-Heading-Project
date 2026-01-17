#ifndef QMC5883L_H
#define QMC5883L_H

#define QMC_ADDRESS      (0x0D << 1)
#define QMC_REG_STATUS   0x06
#define QMC_REG_CR1      0x09
#define QMC_REG_CR2      0x0A
#define QMC_REG_SR		 0x0B
#define QMC_REG_OUT_X_M  0x00
#define QMC_REG_ID       0x0D

typedef struct {
    int6_t Mx;
    int6_t My;
    int6_t Mz;
} QMC5883L_Data_t;

//bitmask for the control register 1 settings
#define MODE_STNDBY      0x00
#define MODE_CONTIN      0x01

#define ODR_10HZ         0x00
#define ODR_50HZ         0x04
#define ODR_100HZ        0x08
#define ODR_200HZ        0x0C

#define RNG_2G           0x00
#define RNG_8G           0x01

#define OSR_512          0x00
#define OSR_256          0x40
#define OSR_128          0x80
#define OSR_64           0xC0

// List of errors
typedef enum {
	QMC5883L_I2C_ERROR,			//0
	QMC5883L_CHIP_ID_ERROR,		//1
    QMC5883L_CONFIG1_ERROR,		//2
	QMC5883L_CONFIG2_ERROR,		//3
	QMC5883L_SR_ERROR,	        //4
	QMC5883L_STATUS_ERROR,	    //5
	QMC5883L_DATAREAD_ERROR		//6
} QMC5883L_ERROR;

HAL_StatusTypeDef QMC_Init(I2C_HandleTypeDef *hi2c, QMC5883L_ERROR *error);
HAL_StatusTypeDef QMC_ReadGauss(I2C_HandleTypeDef *hi2c, QMC5883L_ERROR *error, QMC5883L_Data_t *QMC_data);

#endif
