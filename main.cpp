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
#include "bno055.hpp"

namespace {
#define PERIOD_MS 100
}

static DigitalOut led1(LED1);
static I2C i2c(I2C_SDA, I2C_SCL);
static Serial pc(SERIAL_TX, SERIAL_RX);

static BNO055 bno1(&i2c);

static bno055_accel_t accel;
static bno055_euler_t euler;
uint8_t sys, acc, gyro, mag;

// main() runs in its own thread in the OS
// (note the calls to Thread::wait below for delays)
int main()
{
	pc.baud(115200);

	if (bno1.initialize(BNO055::OperationMode::OperationMode_NDOF, true) != true) {
		pc.printf("ERROR BNO055 not detected. Check your wiring and BNO I2C address\n");
		return 0;
	}
    while (true) {
    	bno1.read_accel(&accel);
    	bno1.get_calibration_status(&sys, &acc, &gyro, &mag);
    	bno1.read_euler(&euler);

    	pc.printf("ACCEL: %.3f %.3f %.3f EULER : %.3f %.3f %.3f Calib: %d% d% d %d\n", accel.x, accel.y, accel.z,
    			euler.x*180/3.1415, euler.y*180/3.1415, euler.z*180/3.1415, sys, acc, gyro, mag);
        led1 = !led1;
        Thread::wait(PERIOD_MS);
    }
}
