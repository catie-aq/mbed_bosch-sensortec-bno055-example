# Bosch Sensortec BNO055 example

[![pipeline status](https://gitlab.com/catie_6tron/bosch-sensortec-bno055-example/badges/master/pipeline.svg)](https://gitlab.com/catie_6tron/bosch-sensortec-bno055-example/commits/master)

A simple demo using a BNO055 to demonstrate some of the data that can be retrieved in fusion mode
and non-fusion mode of the sensor.

Press the **USER BUTTON** to switch between fusion mode and normal mode of the sensor.

In **fusion mode**, the application prints linear accelerations (gravity compensated), Euler
angles, and calibration status:

* Linear Accelerations (X, Y, Z) are the Earth relative accelerations. It is an output of the fusion
  algorithm which compensate the gravity value of the raw accelerations given by the accelerometer.
* Euler angles (X, Y, Z) represent the orientation of the BNO055, which are the result
  of the fusion of the inputs given by the accelerometer, the gyroscope and the
  magnetometer
* Calibration status report the state of calibration of each sensor, from 0 to
  3, 3 being full calibrated. Calibration values are printed in the following
  order : system, accelerometer, gyrometer, magnetometer

In **normal mode**, raw data from the accelerometer, gyroscope and magnetometer are printed.

**Default baudrate** : 115200
