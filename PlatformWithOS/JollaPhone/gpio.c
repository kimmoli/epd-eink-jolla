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
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <err.h>

#include "gpio.h"

/* Private stuff */

volatile unsigned char outputState = 0;

int openDeviceFile(const char* name);
bool writeBytes(unsigned char address, char bytes[], int length);
bool readBytes(unsigned char address, char buffer[], int howManyBytesToRead);
bool i2cWrite(int file, char buffer[], int buffer_length);
bool i2cRead(int file, char buffer[], int howManyBytesToRead);
bool setSlaveAddress(int file, unsigned char address);

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
	/* TODO */
	if (buf[0] && 0x80)
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
		
	printf("Setting outputs to %02x\n", outputState);
	
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

/* Private functions */

int openDeviceFile(const char* name)
{
    int file = open(name, O_RDWR);
    if (file < 0) 
	{
        printf("Device file open failed.\n");
        return -1;
    }
    return file;
}


bool setSlaveAddress(int file, unsigned char address)
{
    if (ioctl(file, I2C_SLAVE, address) < 0) 
	{
        printf("ioctl failed.\n");
        return false;
    }
    return true;
}


bool writeBytes(unsigned char address, char bytes[], int length)
{
    // try to open the device
    int file = openDeviceFile("/dev/i2c-1");

    if (file != -1) 
	{
        // set slave address
        if (setSlaveAddress(file, address)) 
		{
            // try to perform the actual write
            if (i2cWrite(file, bytes, length)) 
			{
                close(file);
                return true;
            }
        }
    }
    close(file);
    return false;
}

bool readBytes(unsigned char address, char buffer[], int howManyBytesToRead)
{
    if (howManyBytesToRead > 255)
    {
        printf("Reading too many bytes.\n");
        return false;
    }

    // try to open the device
    int file = openDeviceFile("/dev/i2c-1");

    if (file != -1) 
	{
        // set slave address
        if (setSlaveAddress(file, address)) 
		{
            // read from the register
            if(i2cRead(file, buffer, howManyBytesToRead)) 
			{
                close(file);
                return true;
            }
        }
    }
    return false;
}


bool i2cWrite(int file, char buffer[], int buffer_length)
{
    if (write(file, buffer, buffer_length) != buffer_length) 
	{
        close(file);
        printf("Write failed.\n");
        return false;
    }

    return true;
}

bool i2cRead(int file, char buffer[], int howManyBytesToRead)
{
    if (read(file, buffer, howManyBytesToRead) != howManyBytesToRead) 
	{
       close(file);
       printf("Read failed.\n");
       return false;
    }

    return true;
}
