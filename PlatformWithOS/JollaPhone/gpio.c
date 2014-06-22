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



#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <err.h>

#include "gpio.h"
#include "jollai2c.h"

/* Private stuff */

volatile unsigned char outputState = 0;


// set up access to the GPIO and PWM
bool GPIO_setup() 
{
	return true;
}

/// revoke access to GPIO and PWM
bool GPIO_teardown() 
{
	/* Nothing to do here */
	return true;
}

void GPIO_mode(GPIO_pin_type pin, GPIO_mode_type mode) 
{
	/* Nothing to do here */
}

int GPIO_read(int pin) 
{
	char buf[2];
	
	buf[0] = GPIO_INPUT_REGISTER;
	
	writeBytes(I2C_ADDRESS, buf, 1);
	readBytes(I2C_ADDRESS, buf, 1);
	
	if (buf[0] & 0x80)
	{		
		return 1;
	} 
	else 
	{
		return 0;
	}
}


void GPIO_write(GPIO_pin_type pin, int value) 
{
	char buf[2];
	
	if (value)
		outputState |= pin;
	else
		outputState &= ~pin;
		
	buf[0] = GPIO_OUTPUT_REGISTER;
	buf[1] = outputState;
	
	writeBytes(I2C_ADDRESS, buf, 2);
}


// only affetct PWM if correct pin is addressed
void GPIO_pwm_write(int pin, uint32_t value) 
{
	/* TODO */
	GPIO_write(GPIO_4, value>0 ? 1:0);
}

