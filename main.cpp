/*
 * Copyright (c) 2016, CATIE, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "mbed.h"
#include "bno055.h"

using namespace sixtron;

namespace {
#define PERIOD_MS 1000
}

void set_configure_sensors(void);

static DigitalOut led1(LED1);
static I2C i2c(I2C_SDA, I2C_SCL);

static BNO055 bno1(&i2c);

static bno055_accelerometer_t accel;
static bno055_euler_t euler;
static bno055_gyroscope_t gyro1;
static bno055_calibration_status_t calib;
static bno055_magnetometer_t mag;

// main() runs in its own thread in the OS
// (note the calls to Thread::wait below for delays)
int main()
{
    if (bno1.initialize(BNO055::OperationMode::NDOF, true) != true) {
    	printf("ERROR BNO055 not detected. Check your wiring and BNO I2C address\n\r");
    	return 0;
    }
    else{
    	// if no fusion mode, here you can configure settings of device
    	if(bno1.operating_mode() < BNO055::OperationMode::IMUPLUS) {
    		// set configure device : acc, gyro and mag sensors
    		set_configure_sensors();
    	}
    	printf("BNO055 configured\n\r");
    }

    while (true) {
        accel = bno1.accelerometer();
        calib = bno1.calibration_status();
        euler = bno1.euler();
    	gyro1 = bno1.gyroscope();
    	mag = bno1.magnetometer();

    	if(bno1.operating_mode() < BNO055::OperationMode::IMUPLUS) {
    	    printf("NO FUSION : ACCEL: %.3f %.3f %.3f GYRO : %.3f %.3f %.3f \n\r", accel.x, accel.y, accel.z,
    	                    gyro1.x, gyro1.y, gyro1.z);
    	} else {
    	    printf("ACCEL: %.3f %.3f %.3f EULER : %.3f %.3f %.3f Calib: %d% d% d %d\n\r", accel.x, accel.y, accel.z,
    			euler.x*180/3.1415, euler.y*180/3.1415, euler.z*180/3.1415, calib.system, calib.accelerometer, calib.gyroscope, calib.magnetometer);
    	}
        led1 = !led1;
        Thread::wait(PERIOD_MS);
    }
}

void set_configure_sensors()
{
    bno1.set_accelerometer_configuration(BNO055::AccelerometerSensorRange::_4G, BNO055::AccelerometerSensorBandWidth::_62Hz, BNO055::AccelerometerSensorOpeMode::Normal);
    bno1.set_gyroscope_configuration(BNO055::GyroscopeSensorRange::_2000DPS, BNO055::GyroscopeSensorBandWidth::_32Hz, BNO055::GyroscopeSensorOpeMode::Normal);
    bno1.set_magnetometer_configuration(BNO055::MagnetometerSensorDataOutputRate::_20Hz, BNO055::MagnetometerSensorOpeMode::EnhancedRegular, BNO055::MagnetometerSensorPowerMode::ForceMode);
}
