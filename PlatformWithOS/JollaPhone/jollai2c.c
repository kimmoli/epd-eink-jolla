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
	printf("spisend\n");
}

// send a data block to SPI and return last bytes returned by slave
// will only change CS if the SPI_CS bits are set
void SPI_read(SPI_type *spi, const void *buffer, void *received, size_t length) 
{
}


// internal functions	
// ==================

static void set_spi_mode(SPI_type *spi, uint8_t mode) 
{
}
