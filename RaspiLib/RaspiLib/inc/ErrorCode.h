#pragma once

namespace PiLib
{

  enum ErrorCode
  {
    E_OK = 0,                   // OK
    E_NOT_INIT,                 // init failed
    E_NOT_IMPLEMENTED,          // function not available

    E_NOACCESS_CPUINFO,         // cannot access /proc/cpuinfo
    E_MISSING_CPUINFO,          // cpuinfo incomplete 

    E_I2C_NOT_INITIALIZED,      // I2C module not initialized
    E_I2C_ERROR_NACK,           // I2C NACK received
    E_I2C_ERROR_CLKT,           // I2C received Clock Stretch Timeout
    E_I2C_ERROR_DATA,           // I2C not all data sent / received


    // component specific errors - RaspiBlocks
    E_BLOCK_NOT_INIT = 100,     // RaspiBlock component not initialized
    E_BLOCK_INVALID_PARAMS,     // invalid parameters 

    E_INVALID_PARAM,            // function called with invalid value
    E_UNEXPECTED_VALUE,         // the value was not expected

  };

}