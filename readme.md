# CS444/544 Automated Greenhouse SCADA ICS project

Authors: Chadwick Goodall, Jennie Tafoya

## Objective

The main objective of this project is to create a miniature scale industrial
control system with four key features:

1. Data Acquisition
2. Data Transmission
3. Data Processing plus display of processed data or taking action based on
the results
4. Use of a standard communication protocol

## Project Description

The project being implemented is an automated greenhouse which will feature automatic plant watering based on soil moisture as well as light-level actuated grow lights. Additionally, temperature and humidity sensors will be utilized to provide information about the internal status of the greenhouse as well as a water level sensor for notifying users when the water resevoir requires refilling. The communication protocol being implemented for interacting with the sensor and actuator peripherals is I2C.

## Components

### Microcontroller

- Elegoo Mega 2560

### Sensors

- Soil moisture sensors
- Temperature and humidity sensors
- Light sensors
- Water level sensor (for water resevoir)

### Actuators

- 5V relay module
- Water pumps
- Grow light LEDs

### HMI

- 20x4 LCD display