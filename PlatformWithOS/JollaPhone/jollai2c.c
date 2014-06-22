// Copyright 2013 Pervasive Displays, Inc.
// Copyright 2014 kimmoli <kimmo.lindholm@eke.fi>
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
#include <unistd.h>
#include <err.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include "jollai2c.h"


// spi information
struct SPI_struct {
	int fd;
	uint32_t bps;
};


// prototypes
static void set_spi_mode(SPI_type *spi, uint8_t mode);


// enable SPI access SPI fd
SPI_type *SPI_create(const char *spi_path, uint32_t bps) 
{

	// allocate memory
	SPI_type *spi = malloc(sizeof(SPI_type));
	if (NULL == spi) 
	{
		warn("falled to allocate SPI structure");
		return NULL;
	}

	/* fake */
	spi->fd = 1;
	spi->bps = bps;

	return spi;
}


// release SPI fd (if open)
bool SPI_destroy(SPI_type *spi) 
{
	if (NULL == spi) 
	{
		return false;
	}
	free(spi);
	return true;
}


// enable SPI, ensures a zero byte was sent (MOSI=0)
// using SPI MODE 2 and that CS and clock remain high
void SPI_on(SPI_type *spi) 
{
	const uint8_t buffer[1] = {0};

#if EPD_COG_VERSION == 1
	set_spi_mode(spi, SPI_MODE_2);
#else
	set_spi_mode(spi, SPI_MODE_0);
#endif
	SPI_send(spi, buffer, sizeof(buffer));
}


// disable SPI, ensures a zero byte was sent (MOSI=0)
// using SPI MODE 0 and that CS and clock remain low
void SPI_off(SPI_type *spi) 
{
	const uint8_t buffer[1] = {0};

	set_spi_mode(spi, SPI_MODE_0);
	SPI_send(spi, buffer, sizeof(buffer));
}

// send a data block to SPI
// will only change CS if the SPI_CS bits are set
void SPI_send(SPI_type *spi, const void *buffer, size_t length) 
{
	int n;
	const char *ptr = ( const char * ) buffer;
	
	char buf[256];
	
	buf[0] = I2C_SPI_BRIDGE;
	
	for (n=0; n<length; n++)
		buf[n+1] = *ptr++;
	
	printf("spisend %d bytes\n", length);
	
	writeBytes(I2C_ADDRESS, buf, length+1);
}

// send a data block to SPI and return last bytes returned by slave
// will only change CS if the SPI_CS bits are set
void SPI_read(SPI_type *spi, const void *buffer, void *received, size_t length) 
{
	/* TODO: This is used only with COG v2 , not needed now */
	printf("SPI_read() not implemented!\n");
}


// internal functions	
// ==================

static void set_spi_mode(SPI_type *spi, uint8_t mode) 
{
	/* Do nothing ?? */
}


// I2C Functions
// ==================

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
