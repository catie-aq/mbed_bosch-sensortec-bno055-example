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
#define PERIOD 100ms
}

void configure_sensors(void);

static DigitalOut led1(LED1);
static I2C i2c(I2C1_SDA, I2C1_SCL);
static InterruptIn button(BUTTON1);

static BNO055 bno(&i2c);

static bno055_linear_acceleration_t laccel;
static bno055_acceleration_t accel;
static bno055_euler_t euler;
static bno055_angular_velocity_t gyro;
static bno055_calibration_status_t calib;
static bno055_magnetic_field_t mag;

static EventQueue queue;

static bool use_fusion = true;

/*!
 * \brief User button interruption used to change IMU mode
 */
void button_interrupt()
{
    use_fusion = !use_fusion;
    queue.call(printf, "\nSWITCH IMU MODE TO: %s!\n", (use_fusion? "FUSION":"NORMAL"));
    queue.call(configure_sensors);
}

/*!
 * \brief print sensor data, depending on the sensor modes. Regularly called by the event queue 
 */
void print_sensor_data()
{
    if (use_fusion) {
        laccel = bno.linear_acceleration();
        calib = bno.calibration_status();
        euler = bno.euler();
        gyro = bno.angular_velocity();
        mag = bno.magnetic_field();

        printf("LINEAR ACCELERATION (m/s²): %6.3f %6.3f %6.3f \tEULER (°): %6.3f %6.3f %6.3f \tCALIBRATION: %d %d %d %d    \r",
            laccel.x, laccel.y, laccel.z, 
            euler.x*180/3.1415, euler.y*180/3.1415, euler.z*180/3.1415,
            calib.system, calib.accelerometer, calib.gyroscope, calib.magnetometer);
        fflush(stdout);
    } else {
        accel = bno.acceleration();
        gyro = bno.angular_velocity();
        mag = bno.magnetic_field();

        printf("ACCELERATION (m/s²): %6.3f %6.3f %6.3f\tGYROSCOPE (°/s): %6.3f %6.3f %6.3f\tMAGNETOMETER (µT): %6.3f %6.3f %6.3f    \r",
            accel.x, accel.y, accel.z, gyro.x, gyro.y, gyro.z, mag.x, mag.y, mag.z);
        fflush(stdout);
    }

    led1 = !led1;
}

int main()
{
    printf("6TRON BNO055 Example\n");
    printf("Press the USER BUTTON at any time to enable/disable fusion mode of the IMU\n");

    if (bno.initialize(BNO055::OperationMode::NDOF, true) != true) {
        printf("ERROR BNO055 not detected. Check your wiring and BNO I2C address\n\r");
        return 0;
    }
    printf("BNO055 configured in fusion mode!\n\r");

    button.fall(button_interrupt);

    ThisThread::sleep_for(1s);

    queue.call_every(PERIOD, print_sensor_data);
    queue.dispatch_forever();
}

/*!
 * \brief Configure sensors for fusion or non-fusion mode
 */
void configure_sensors()
{
    if (use_fusion) {
        bno.set_operation_mode(BNO055::OperationMode::NDOF);
    } else {
        bno.set_operation_mode(BNO055::OperationMode::AMG);

        bno.set_accelerometer_configuration(BNO055::AccelerometerSensorRange::_4G,
            BNO055::AccelerometerSensorBandWidth::_500Hz,
            BNO055::AccelerometerSensorOperationMode::Normal);
        bno.set_gyroscope_configuration(BNO055::GyroscopeSensorRange::_2000DPS,
            BNO055::GyroscopeSensorBandWidth::_116Hz,
            BNO055::GyroscopeSensorOperationMode::Normal);
        bno.set_magnetometer_configuration(BNO055::MagnetometerSensorDataOutputRate::_20Hz,
            BNO055::MagnetometerSensorOperationMode::HighAccuracy,
            BNO055::MagnetometerSensorPowerMode::Normal);
    }
}
