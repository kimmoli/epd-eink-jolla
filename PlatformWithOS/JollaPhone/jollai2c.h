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


#if !defined(JOLLAI2C_H)
#define JOLLAI2C_H 1

#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

#define I2C_ADDRESS 0x24
#define GPIO_OUTPUT_REGISTER 0x02
#define GPIO_INPUT_REGISTER 0x01
#define I2C_SPI_BRIDGE 0x10


// type to hold SPI data
typedef struct SPI_struct SPI_type;


// functions wrapped from SPI to I2C really...
// =========

// enable SPI access SPI fd
SPI_type *SPI_create(const char *spi_path, uint32_t bps);

// release SPI fd
bool SPI_destroy(SPI_type *spi);

// enable SPI, ensures a zero byte was sent (MOSI=0)
// using SPI MODE 2 and that CS and clock remain high
void SPI_on(SPI_type *spi);

// disable SPI, ensures a zero byte was sent (MOSI=0)
// using SPI MODE 0 and that CS and clock remain low
void SPI_off(SPI_type *spi);

// send a data block to SPI
// will only change CS if the SPI_CS bits are set
void SPI_send(SPI_type *spi, const void *buffer, size_t length);

// send a data block to SPI and return last bytes returned by slave
// will only change CS if the SPI_CS bits are set
void SPI_read(SPI_type *spi, const void *buffer, void *received, size_t length);

// Actual I2C functions

int openDeviceFile(const char* name);
bool writeBytes(unsigned char address, char bytes[], int length);
bool readBytes(unsigned char address, char buffer[], int howManyBytesToRead);
bool i2cWrite(int file, char buffer[], int buffer_length);
bool i2cRead(int file, char buffer[], int howManyBytesToRead);
bool setSlaveAddress(int file, unsigned char address);


#endif /* JOLLAI2C */
