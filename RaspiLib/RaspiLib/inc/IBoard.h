#pragma once

#include "ErrorCode.h"
#include "BoardInfo.h"

namespace PiLib
{
  // Raspberry Pi board interface
  class IBoard
  {
    public:

      virtual const BoardInfo & GetBoardInfo() = 0;


  };

};