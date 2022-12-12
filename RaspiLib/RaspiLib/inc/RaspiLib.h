#pragma once

#include "IPin.h"
#include "IBoard.h"
#include "II2c.h"

#include "Utils.h"

#include <map>

namespace PiLib
{
  /**
   * @brief The main library entry point
   * 
   */
  class RaspiLib
  {
    public:
      // static access functions
      // return true on success, if false, do not use lib!
      static bool Startup();
      static bool Shutdown();

      static void LibVersion(unsigned &major, unsigned &minor);

      // singleton, there is only one instance per application
      static RaspiLib &GetInstance();

      // object related stuff
      IBoard &GetBoard();
      // one single IO pin
      IPin &GetPin(RaspiPinLabel label);
      // the I2C bus - this one locks PIN3_GPIO2_SDA and PIN5_GPIO3_SCL
      II2c &GetI2c();

    private:

      RaspiLib();
      ~RaspiLib();

      // init object
      bool Init();
      ErrorCode InsertGPIOPins();

      // internal object
      static RaspiLib *m_instance;

      // the board context
      IBoard *m_board = nullptr;
      std::map<RaspiPinLabel, IPin *> m_myPins;
      II2c *m_i2c = nullptr;

      // if something is wrong with init, make sure all pointers are released
      void cleanup();
  };

}