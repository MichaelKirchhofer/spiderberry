#pragma once

#include "IBoard.h"

namespace PiLib
{

  class RaspiBoard : public IBoard
  {
    public:
      RaspiBoard();
      virtual ~RaspiBoard();

      ErrorCode Init();

      const BoardInfo & GetBoardInfo() {return m_board;}


    private:
      BoardInfo m_board;

      ErrorCode ReadBoardInfo();
  };

};