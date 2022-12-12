/*
    smbus.c - SMBus level access helper functions

    Copyright (C) 1995-1997  Simon G. Vogl
    Copyright (C) 1998-1999  Frodo Looijaard <frodol@dds.nl>
    Copyright (C) 2012-2013  Jean Delvare <jdelvare@suse.de>

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.
*/

#include <errno.h>
#include <string.h>
#include <stddef.h>
#include <smbus.h>

#include <linux/types.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#include <cstdio>
#include <unistd.h>

#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

/* Compatibility defines */
#ifndef I2C_SMBUS_I2C_BLOCK_BROKEN
#define I2C_SMBUS_I2C_BLOCK_BROKEN I2C_SMBUS_I2C_BLOCK_DATA
#endif
#ifndef I2C_FUNC_SMBUS_PEC
#define I2C_FUNC_SMBUS_PEC I2C_FUNC_SMBUS_HWPEC_CALC
#endif

// this one is not published
int32_t i2c_smbus_access(int file, char read_write, uint8_t command, int size, union i2c_smbus_data *data)
{
	struct i2c_smbus_ioctl_data args;
	int32_t err;

	args.read_write = read_write;
	args.command = command;
	args.size = size;
	args.data = data;

	err = ioctl(file, I2C_SMBUS, &args);
	if (err == -1)
		err = -errno;
	return err;
}

int32_t i2c_smbus_write_quick(int file, uint8_t value)
{
	return i2c_smbus_access(file, value, 0, I2C_SMBUS_QUICK, NULL);
}

int32_t i2c_smbus_read_byte(int file)
{
	union i2c_smbus_data data;
	int err;

	err = i2c_smbus_access(file, I2C_SMBUS_READ, 0, I2C_SMBUS_BYTE, &data);
	if (err < 0)
		return err;

	return 0x0FF & data.byte;
}

int32_t i2c_smbus_write_byte(int file, uint8_t value)
{
	return i2c_smbus_access(file, I2C_SMBUS_WRITE, value,
				I2C_SMBUS_BYTE, NULL);
}

int32_t i2c_smbus_read_byte_data(int file, uint8_t command)
{
	union i2c_smbus_data data;
	int err;

	err = i2c_smbus_access(file, I2C_SMBUS_READ, command,
			       I2C_SMBUS_BYTE_DATA, &data);
	if (err < 0)
		return err;

	return 0x0FF & data.byte;
}

int32_t i2c_smbus_write_byte_data(int file, uint8_t command, uint8_t value)
{
	union i2c_smbus_data data;
	data.byte = value;
	return i2c_smbus_access(file, I2C_SMBUS_WRITE, command,
				I2C_SMBUS_BYTE_DATA, &data);
}

int32_t i2c_smbus_read_word_data(int file, uint8_t command)
{
	union i2c_smbus_data data;
	int err;

	err = i2c_smbus_access(file, I2C_SMBUS_READ, command,
			       I2C_SMBUS_WORD_DATA, &data);
	if (err < 0)
		return err;

	return 0x0FFFF & data.word;
}

int32_t i2c_smbus_write_word_data(int file, uint8_t command, uint16_t value)
{
	union i2c_smbus_data data;
	data.word = value;
	return i2c_smbus_access(file, I2C_SMBUS_WRITE, command,
				I2C_SMBUS_WORD_DATA, &data);
}

int32_t i2c_smbus_process_call(int file, uint8_t command, uint16_t value)
{
	union i2c_smbus_data data;
	data.word = value;
	if (i2c_smbus_access(file, I2C_SMBUS_WRITE, command,
			     I2C_SMBUS_PROC_CALL, &data))
		return -1;
	else
		return 0x0FFFF & data.word;
}

/* Returns the number of read bytes */
int32_t i2c_smbus_read_block_data(int file, uint8_t command, uint8_t *values)
{
	union i2c_smbus_data data;
	int i, err;

	err = i2c_smbus_access(file, I2C_SMBUS_READ, command,
			       I2C_SMBUS_BLOCK_DATA, &data);
	if (err < 0)
		return err;

	for (i = 1; i <= data.block[0]; i++)
		values[i-1] = data.block[i];
	return data.block[0];
}

int32_t i2c_smbus_write_block_data(int file, uint8_t command, uint8_t length, const uint8_t *values)
{
	union i2c_smbus_data data;
	int i;
	if (length > I2C_SMBUS_BLOCK_MAX)
		length = I2C_SMBUS_BLOCK_MAX;
	for (i = 1; i <= length; i++)
		data.block[i] = values[i-1];
	data.block[0] = length;
	return i2c_smbus_access(file, I2C_SMBUS_WRITE, command,
				I2C_SMBUS_BLOCK_DATA, &data);
}

/* Returns the number of read bytes */
/* Until kernel 2.6.22, the length is hardcoded to 32 bytes. If you
   ask for less than 32 bytes, your code will only work with kernels
   2.6.23 and later. */
int32_t i2c_smbus_read_i2c_block_data(int file, uint8_t command, uint8_t length, uint8_t *values)
{
	union i2c_smbus_data data;
	int i, err;

	if (length > I2C_SMBUS_BLOCK_MAX)
		length = I2C_SMBUS_BLOCK_MAX;
	data.block[0] = length;

	err = i2c_smbus_access(file, I2C_SMBUS_READ, command,
			       length == 32 ? I2C_SMBUS_I2C_BLOCK_BROKEN :
				I2C_SMBUS_I2C_BLOCK_DATA, &data);
	if (err < 0)
		return err;

	for (i = 1; i <= data.block[0]; i++)
		values[i-1] = data.block[i];
	return data.block[0];
}

int32_t i2c_smbus_write_i2c_block_data(int file, uint8_t command, uint8_t length, const uint8_t *values)
{
	union i2c_smbus_data data;
	int i;
	if (length > I2C_SMBUS_BLOCK_MAX)
		length = I2C_SMBUS_BLOCK_MAX;
	for (i = 1; i <= length; i++)
		data.block[i] = values[i-1];
	data.block[0] = length;
	return i2c_smbus_access(file, I2C_SMBUS_WRITE, command,
				I2C_SMBUS_I2C_BLOCK_BROKEN, &data);
}

/* Returns the number of read bytes */
int32_t i2c_smbus_block_process_call(int file, uint8_t command, uint8_t length, uint8_t *values)
{
	union i2c_smbus_data data;
	int i, err;

	if (length > I2C_SMBUS_BLOCK_MAX)
		length = I2C_SMBUS_BLOCK_MAX;
	for (i = 1; i <= length; i++)
		data.block[i] = values[i-1];
	data.block[0] = length;

	err = i2c_smbus_access(file, I2C_SMBUS_WRITE, command,
			       I2C_SMBUS_BLOCK_PROC_CALL, &data);
	if (err < 0)
		return err;

	for (i = 1; i <= data.block[0]; i++)
		values[i-1] = data.block[i];
	return data.block[0];
}

// add functions to abstract OS here - return file
int init_smbus(int bus)
{
	int file, len;
  char filename[20];

	len = snprintf(filename, sizeof(filename), "/dev/i2c-%d", bus);
	if (len >= (int) sizeof(filename)) 
	{
	  fprintf(stderr, "%s: path truncated\n", filename);
		return -EOVERFLOW;
	}
	
	file = open(filename, O_RDWR);

	if (file < 0 ) 
	{
		if (errno == ENOENT) 
		{
			fprintf(stderr, "Error: Could not open file %s: %s\n",
				filename, strerror(ENOENT));
		} 
		else 
		{
			fprintf(stderr, "Error: Could not open file %s: %s\n", filename, strerror(errno));
			if (errno == EACCES)
				fprintf(stderr, "Run as root?\n");
		}
	}

	return file;
}

int close_smbus(int file)
{
	if (file >= 0)
	  close(file);

	return 0;
}

int setSlaveAddress(int file, uint8_t address)
{
	if (file < 0) return -1;

	if (ioctl(file, I2C_SLAVE, address) < 0)
	{
		if (errno == EBUSY) return 0;
		else return -1;
	}

	return 1;
}
