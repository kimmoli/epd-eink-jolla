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


// set up access to the GPIO and PWM
bool GPIO_setup() 
{
	return true;
}

/// revoke access to GPIO and PWM
bool GPIO_teardown() 
{
	return true;
}

void GPIO_mode(GPIO_pin_type pin, GPIO_mode_type mode) 
{
}

int GPIO_read(int pin) 
{
	/* TODO */
	{		
		return 0;
	} 
	else 
	{
		return 1;
	}
}


void GPIO_write(GPIO_pin_type pin, int value) 
{
	/* TODO */
}


// only affetct PWM if correct pin is addressed
void GPIO_pwm_write(int pin, uint32_t value) 
{
	/* TODO */
	GPIO_write(busy_pin, value>0 ? 1:0);
}

