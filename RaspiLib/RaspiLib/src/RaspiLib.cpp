#include "RaspiLib.h"
#include "Version.h"

#include "RaspiBoard.h"
#include "Pin.h"
#include "I2C.h"

#include <iostream>

using namespace PiLib;

RaspiLib *RaspiLib::m_instance = nullptr;

// static access functions
bool RaspiLib::Startup()
{
  // create board instance & init
  m_instance = new RaspiLib();

  return m_instance->Init();
}

bool RaspiLib::Shutdown()
{
  return true;
}

void RaspiLib::LibVersion(unsigned &major, unsigned &minor)
{
  major = MAJOR_VERSION;
  minor = MINOR_VERSION;
}

// singleton, there is only one instance per application
RaspiLib &RaspiLib::GetInstance()
{
  return *m_instance;
}

// -------------------------------------------------------------------

// object related stuff
IBoard &RaspiLib::GetBoard()
{
  return *m_board;
}

// the fallback pin
Pin dummy(PIN_UNDEFINED);

IPin &RaspiLib::GetPin(RaspiPinLabel label)
{
  if (label >= PIN_SENTINEL) return dummy;

  auto it = m_myPins.find(label);
  if (it != m_myPins.end())
  {
    return *m_myPins[label];
  }

  return dummy;
}

II2c &RaspiLib::GetI2c()
{
  return *m_i2c;
}

RaspiLib::RaspiLib()
{

}

RaspiLib::~RaspiLib()
{
  
}

bool RaspiLib::Init()
{
  // make sure we init only once
  if (m_board && m_i2c) return true;

  RaspiBoard *ptr = new RaspiBoard();

  ErrorCode err = ptr->Init();
  if (err == E_OK)
    m_board = ptr;
  else 
  {
    std::cerr << "board init got error " << err << std::endl;
    return false;   // do no further setup
  }

  err = InsertGPIOPins();
  if (err != E_OK)
  {
    std::cerr << "gpio pins init got error " << err << std::endl;
    cleanup();
    // error
    return false;
  }

  I2C *iptr = new I2C();
  err = iptr->init();
  if (err == E_OK)
    m_i2c = iptr;
  else
  {
    std::cerr << "i2c init got error " << err << std::endl;
    cleanup();
    return false;
  }

  return true;
}

ErrorCode RaspiLib::InsertGPIOPins()
{
  // find all valid GPIO pins & insert them into pin map
  for (int index = PIN_UNDEFINED; index < PIN_SENTINEL; index++)
  {
    if (Pin::IsValidPin((RaspiPinLabel)index))
      m_myPins[(RaspiPinLabel)index] = new Pin((RaspiPinLabel)index);
  }

  return E_OK;
}

void RaspiLib::cleanup()
{
  // delete i2c instance
  delete m_i2c;
  m_i2c = nullptr;

  // remove pins
  m_myPins.clear();

  // delete board
  delete m_board;
  m_board = nullptr;
}