#pragma once

#include <string>

namespace PiLib
{
  enum PI_Processor
  {
    PI_PROCESSOR_UNKNOWN,
    PI_PROCESSOR_BCM2835,
  };

  struct BoardInfo
  {
    std::string model = "unknown";
    unsigned revision = 0;
    PI_Processor processor = PI_PROCESSOR_UNKNOWN;

    std::string ToString() const;
  };

};