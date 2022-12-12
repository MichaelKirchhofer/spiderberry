#pragma once

#include <cstdint>
#include <ErrorCode.h>

namespace SpiderLib
{
  // interface for setting the servos to a certain angle
  // we use the channel numbers in the legs
  // channel to device is handled by the Spider class internally
  class SpiderServos
  {
    public:
      virtual PiLib::ErrorCode SetServoAngle(std::uint8_t channel, float angle) = 0;
  };
}