# STM32 Nucleo L476RG – MPU6050 + QMC5883L Tilt-Compensated Heading

This project runs on the **STM32 Nucleo-L476RG** and reads:
- **MPU6050** (accelerometer + gyroscope) over **I2C1**
- **QMC5883L** (3-axis magnetometer) over **I2C1**

It then computes a **tilt-compensated compass heading** (roll/pitch from accel, yaw from mag) and streams the sensor values + heading over **UART @ 115200 baud**.

---

## Features

### MPU6050 (I2C)
- Reads **14 bytes** starting at `ACCEL_XOUT_H`
- Outputs:
  - `ax, ay, az` (raw `int16_t`)
  - `gx, gy, gz` (raw `int16_t`)
  - `temp` (raw `int16_t`)
- Configuration performed in `MPU6050_init()`:
  - Reset + wake / clock select via `PWR_MGMT_1`
  - Digital low-pass filter via `CONFIG`
  - Sample rate divider via `SMPLRT_DIV`
  - Gyro range via `GYRO_CONFIG`
  - Accel range via `ACCEL_CONFIG`

### QMC5883L (I2C)
- `QMC_Init()`:
  - Reads chip ID from `QMC_REG_ID` (expects `0xFF` in this driver)
  - Writes config to:
    - `QMC_REG_CR1` (OSR/RNG/ODR/MODE)
    - `QMC_REG_CR2` (set to `0x00`)
    - `QMC_REG_SR` (set to `0x01`)
- `QMC_ReadGauss()`:
  - Reads `QMC_REG_STATUS`
  - If **data ready** (`status & 0x01`) is set, reads **6 bytes**
  - Decodes raw `int16_t`:
    - `Mx, My, Mz`

> Note: despite the function name `QMC_ReadGauss`, the current code stores **raw readings** (not scaled to Gauss).

### Tilt-Compensated Heading
- Accelerometer is normalized
- Roll and pitch are calculated from accel
- Magnetometer is tilt-compensated using roll/pitch
- Heading is computed using `atan2` and converted to degrees `[0..360)`

---

## Hardware

### Required Parts
- STM32 **Nucleo-L476RG**
- **MPU6050** I2C module
- **QMC5883L** I2C module
- USB cable (power / programming / serial output)

### Wiring (I2C1 shared)
| Signal | Nucleo L476RG | MPU6050 | QMC5883L |
|---|---|---|---|
| SDA | PB7 (I2C1_SDA) | SDA | SDA |
| SCL | PB6 (I2C1_SCL) | SCL | SCL |
| VCC | 3.3V | VCC | VCC |
| GND | GND | GND | GND |

**Notes**
- Make sure your modules are **3.3V compatible**
- I2C needs pull-ups; many breakout boards include them (if not, add ~4.7k to 3.3V)

---

## Project Structure

```text
Core/
├── Inc/
│   ├── main.h
│   ├── MPU6050.h
│   └── QMC5883L.h
└── Src/
    ├── main.c
    ├── MPU6050.c
    └── QMC5883L.c
