// Copyright 2013 Pervasive Displays, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at:
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
// express or implied.  See the License for the specific language
// governing permissions and limitations under the License.


#if !defined(GPIO_H)
#define GPIO_H 1

#include <stdbool.h>
#include <stdint.h>

#define I2C_ADDRESS 0x24
#define GPIO_OUTPUT_REGISTER 2
#define GPIO_INPUT_REGISTER 1

// pin types
typedef enum {
	GPIO_0 = 0x01,	/* EINK_RESETL */
	GPIO_1 = 0x02,  /* EINK_ON */
	GPIO_2 = 0x04,  /* EINK_BORDER */
	GPIO_3 = 0x08,  /* EINK_DISCHARGE */
	GPIO_4 = 0x10,  /* EINK_PWM */
	GPIO_5 = 0x80,  /* EINK_BUSY */
	GPIO_6 = 0x20   /* Red led */
} GPIO_pin_type;

// GPIO modes
typedef enum {
        GPIO_INPUT,   // as input
        GPIO_OUTPUT,  // as output
        GPIO_PWM      // as PWM output (only for P1_12
} GPIO_mode_type;

// functions
// =========

// enable GPIO system (maps device registers)
// return false if failure
bool GPIO_setup();

// release mapped device registers
bool GPIO_teardown();

// set a mode for a given GPIO pin
void GPIO_mode(GPIO_pin_type pin, GPIO_mode_type mode);

// return a value (0/1) for a given input pin
int GPIO_read(int pin);

// set or clear a given output pin
void GPIO_write(GPIO_pin_type pin, int value);

// set the PWM ration 0..1023 for hardware PWM pin (GPIO_P1_12)
void GPIO_pwm_write(int pin, uint32_t value);


#endif
