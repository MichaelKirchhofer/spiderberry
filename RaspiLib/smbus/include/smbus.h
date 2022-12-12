/*
    smbus.h - SMBus level access helper functions, reworked for RaspiLib
*/

#pragma once

#define I2C_API_VERSION		0x100

#include <cstdint>

// internal function
// int32_t i2c_smbus_access(int file, char read_write, __u8 command,
// 			      int size, union i2c_smbus_data *data);

int32_t i2c_smbus_write_quick(int file, uint8_t value);
int32_t i2c_smbus_read_byte(int file);
int32_t i2c_smbus_write_byte(int file, uint8_t value);
int32_t i2c_smbus_read_byte_data(int file, uint8_t command);
int32_t i2c_smbus_write_byte_data(int file, uint8_t command, uint8_t value);
int32_t i2c_smbus_read_word_data(int file, uint8_t command);
int32_t i2c_smbus_write_word_data(int file, uint8_t command, uint16_t value);
int32_t i2c_smbus_process_call(int file, uint8_t command, uint16_t value);

/* Returns the number of read bytes */
int32_t i2c_smbus_read_block_data(int file, uint8_t command, uint8_t *values);
int32_t i2c_smbus_write_block_data(int file, uint8_t command, uint8_t length, const uint8_t *values);

/* Returns the number of read bytes */
/* Until kernel 2.6.22, the length is hardcoded to 32 bytes. If you
   ask for less than 32 bytes, your code will only work with kernels
   2.6.23 and later. */
int32_t i2c_smbus_read_i2c_block_data(int file, uint8_t command, uint8_t length, uint8_t *values);
int32_t i2c_smbus_write_i2c_block_data(int file, uint8_t command, uint8_t length, const uint8_t *values);

/* Returns the number of read bytes */
int32_t i2c_smbus_block_process_call(int file, uint8_t command, uint8_t length,  uint8_t *values);

// add functions to abstract OS here - return file
// hide file operations in here, just provide file handle
int init_smbus(int bus);
int close_smbus(int file);

int setSlaveAddress(int file, uint8_t address);
