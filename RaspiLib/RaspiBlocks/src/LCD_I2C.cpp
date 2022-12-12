#include "LCD_I2C.h"

#include <RaspiLib.h>

using namespace PiLib;

// PCF8574T 8bit DIO
// address 0x27

// bit 7 - D3
// bit 6 - D2
// bit 5 - D1
// bit 4 - D0

// bit 3 - nibble, always 1
// bit 2 - EN
// bit 1 - RW
// bit 0 - RS

const int NIBBLE = 0x08;
const int EN = 0x04;
const int RW = 0x02;
const int RS = 0x01;
const int EN_MASK = 0xFB;

LCD_I2C::LCD_I2C(int address)
{
  m_address = address;
}

LCD_I2C::~LCD_I2C()
{}

ErrorCode LCD_I2C::Init()
{
  SendCommand(0x33); // init 8bit mode first
  Utils::delay_ms(5);
  SendCommand(0x33); // init 8bit mode first
  Utils::delay_ms(5);
  SendCommand(0x33); // init 8bit mode first
  Utils::delay_ms(5);
  SendCommand(0x32); // init 4bit mode
  Utils::delay_ms(5);
  SendCommand(0x28); // 2 lines, 5x7 dots
  Utils::delay_ms(5);
  SendCommand(0x0C); // enable display, no cursor
  Utils::delay_ms(5);

  Clear();

  return E_OK;
}

ErrorCode LCD_I2C::Clear()
{
  SendCommand(0x01); // clear screen
  Utils::delay_ms(5);

  return E_OK;
}

ErrorCode LCD_I2C::Write(int x, int y, std::string text)
{
  if (x < 0)  x = 0;
	if (x > 15) x = 15;
	if (y < 0)  y = 0;
	if (y > 1)  y = 1;

  // move cursor
  int loc = 0x80 + 0x40*y + x;
  SendCommand(loc);

  for (auto c : text)
    SendData(c);

  return E_OK; 
}

ErrorCode LCD_I2C::SendCommand(int command)
{
  int buf = command & 0xF0; // high nibble first
  buf |= EN;  // RS = 0, RW = 0, EN = 1
  WriteWord(buf);
  Utils::delay_ms(2);
  buf &= EN_MASK;  // EN = 0
  WriteWord(buf);
  Utils::delay_ms(2);

  buf = (command & 0x0F) << 4; // low nibble
  buf |= EN;  // RS = 0, RW = 0, EN = 1
  WriteWord(buf);
  Utils::delay_ms(2);
  buf &= EN_MASK;  // EN = 0
  WriteWord(buf);

  return E_OK; 
}

ErrorCode LCD_I2C::SendData(int data)
{
  int buf = data & 0xF0; // high nibble first
  buf |= (EN + RS);  // RS = 1, RW = 0, EN = 1
  WriteWord(buf);
  Utils::delay_ms(2);
  buf &= EN_MASK;  // EN = 0
  WriteWord(buf);
  Utils::delay_ms(2);

  buf = (data & 0x0F) << 4; // low nibble
  buf |= (EN + RS);  // RS = 1, RW = 0, EN = 1
  WriteWord(buf);
  Utils::delay_ms(2);
  buf &= EN_MASK;  // EN = 0
  WriteWord(buf);

  return E_OK; 
}

ErrorCode LCD_I2C::WriteWord(int word)
{
  word |= NIBBLE;

  II2c &iface = RaspiLib::GetInstance().GetI2c();
  
  uint8_t bb = word & 0xFF;

  return iface.WriteByte(m_address, bb);
}

