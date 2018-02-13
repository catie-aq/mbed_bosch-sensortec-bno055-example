# Bosch Sensortec BNO055 example

[![pipeline status](https://gitlab.com/catie_6tron/bosch-sensortec-bno055-example/badges/master/pipeline.svg)](https://gitlab.com/catie_6tron/bosch-sensortec-bno055-example/commits/master)

A demo using a BNO055 in fusion mode which print raw acceleration, Euler
angles, and calibration status.

* Acceleration (X, Y, Z) are raw data, given by the integrated accelerometer
* Euler angles (X, Y, Z) are computed data by the BNO055, which are the result
  of the fusion of the inputs given by the accelerometer, the gyrometer and the
  magnetometer
* Calibration status report the state of calibration of each sensor, from 0 to
  3, 3 being full callibrated. Calibration values are printed in the following
  order : system, accelerometer, gyrometer, magnetometer

