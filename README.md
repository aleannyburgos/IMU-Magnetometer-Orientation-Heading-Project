# STM32 Nucleo L476RG – IMU + Magnetometer Heading Calculation

This project demonstrates how to read motion-sensor data from the **MPU6050** (accelerometer & gyroscope) and **QMC5883L** (3-axis magnetometer) using the STM32 Nucleo-L476RG, then compute a **tilt-compensated electronic compass heading**.

The microcontroller sends acceleration, gyro, magnetometer readings, and the calculated heading over UART at 115200 baud.

---

## Features

- **MPU6050** via I2C  
  - Acceleration (Ax, Ay, Az)  
  - Gyroscope (Gx, Gy, Gz)

- **QMC5883L Magnetometer** via I2C  
  - Magnetic field (Mx, My, Mz)

- **Tilt-Compensated Heading Calculation**
  - Uses accelerometer to estimate roll & pitch  
  - Uses magnetometer to compute a corrected compass heading  
  - Works while the board is not perfectly level

- **UART output**
  - Prints sensor values and heading to a serial monitor (115200 baud)

---

## Hardware Setup

### Required Components
- STM32 **Nucleo-L476RG** board  
- MPU6050 module (I2C)  
- QMC5883L magnetometer module (I2C)  
- USB cable for programming + serial output  

### Wiring

Both sensors can share **I2C1**:

| Sensor | Pin | Nucleo Pin |
|--------|-----|-----------|
| MPU6050 | SDA | PB7 (I2C1_SDA) |
| MPU6050 | SCL | PB6 (I2C1_SCL) |
| QMC5883L | SDA | PB7 (I2C1_SDA) |
| QMC5883L | SCL | PB6 (I2C1_SCL) |
| All | GND | GND |
| All | VCC | 3.3V |

---

## Firmware Summary

### MPU6050 Driver
Located in: MPU6050.c / MPU6050.h

### QMC5883L Driver
Located in: QMC5883L.c / QMC5883L.h
