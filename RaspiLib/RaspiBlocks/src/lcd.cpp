#include "lcd.h"

#include <RaspiLib.h>
#include <Utils.h>

using namespace PiLib;
using namespace std;

// HD44780U Commands

#define	LCD_CLEAR	0x01
#define	LCD_HOME	0x02
#define	LCD_ENTRY	0x04
#define	LCD_CTRL	0x08
#define	LCD_CDSHIFT	0x10
#define	LCD_FUNC	0x20
#define	LCD_CGRAM	0x40
#define	LCD_DGRAM	0x80

// Bits in the entry register

#define	LCD_ENTRY_SH		0x01
#define	LCD_ENTRY_ID		0x02

// Bits in the control register

#define	LCD_BLINK_CTRL		0x01
#define	LCD_CURSOR_CTRL		0x02
#define	LCD_DISPLAY_CTRL	0x04

// Bits in the function register

#define	LCD_FUNC_F	0x04
#define	LCD_FUNC_N	0x08
#define	LCD_FUNC_DL	0x10

#define	LCD_CDSHIFT_RL	0x04

// width 16 or 20, default 16
// height 1, 2 or 4, default 2
// any other value will be set to default 
LCD::LCD(int width, int height) : m_rows(height), m_cols(width)
{
  // fallback
  if (m_cols != 16 && m_cols != 20) m_cols = 16;
  if (m_rows != 1 && m_rows != 2 && m_rows != 4) m_rows = 2;
}

LCD::~LCD()
{}

ErrorCode LCD::init(bool nibble, RaspiPinLabel rs, RaspiPinLabel strb, 
                RaspiPinLabel d0, RaspiPinLabel d1, RaspiPinLabel d2, RaspiPinLabel d3,
                RaspiPinLabel d4, RaspiPinLabel d5, RaspiPinLabel d6, RaspiPinLabel d7)
{
  ErrorCode err = E_OK;

  m_bits = 8;
  if (nibble) m_bits = 4;

  RaspiLib &theLib = RaspiLib::GetInstance();

  RS_PIN = & theLib.GetPin(rs);
  STRB_PIN = & theLib.GetPin(strb);

  D0_PIN = & theLib.GetPin(d0);
  D1_PIN = & theLib.GetPin(d1);
  D2_PIN = & theLib.GetPin(d2);
  D3_PIN = & theLib.GetPin(d3);
  D4_PIN = & theLib.GetPin(d4);
  D5_PIN = & theLib.GetPin(d5);
  D6_PIN = & theLib.GetPin(d6);
  D7_PIN = & theLib.GetPin(d7);

  // sanity checks
  if (RS_PIN->GetGPIONumber() == GPIO_DUMMY) err = E_BLOCK_NOT_INIT;
  if (STRB_PIN->GetGPIONumber() == GPIO_DUMMY) err = E_BLOCK_NOT_INIT;
  if (D0_PIN->GetGPIONumber() == GPIO_DUMMY) err = E_BLOCK_NOT_INIT;
  if (D1_PIN->GetGPIONumber() == GPIO_DUMMY) err = E_BLOCK_NOT_INIT;
  if (D2_PIN->GetGPIONumber() == GPIO_DUMMY) err = E_BLOCK_NOT_INIT;
  if (D3_PIN->GetGPIONumber() == GPIO_DUMMY) err = E_BLOCK_NOT_INIT;

  if (m_bits == 8)
  {
    if (D4_PIN->GetGPIONumber() == GPIO_DUMMY) err = E_BLOCK_NOT_INIT;
    if (D5_PIN->GetGPIONumber() == GPIO_DUMMY) err = E_BLOCK_NOT_INIT;
    if (D6_PIN->GetGPIONumber() == GPIO_DUMMY) err = E_BLOCK_NOT_INIT;
    if (D7_PIN->GetGPIONumber() == GPIO_DUMMY) err = E_BLOCK_NOT_INIT;
  }

  if (err != E_OK) return err;

  // now init the pins
  RS_PIN->WriteBool(false); RS_PIN->SetPinMode(FSEL_OUTP);
  STRB_PIN->WriteBool(false); STRB_PIN->SetPinMode(FSEL_OUTP);

  D0_PIN->WriteBool(false); D0_PIN->SetPinMode(FSEL_OUTP);
  D1_PIN->WriteBool(false); D1_PIN->SetPinMode(FSEL_OUTP);
  D2_PIN->WriteBool(false); D2_PIN->SetPinMode(FSEL_OUTP);
  D3_PIN->WriteBool(false); D3_PIN->SetPinMode(FSEL_OUTP);

  if (m_bits == 8)
  {
    D4_PIN->WriteBool(false); D4_PIN->SetPinMode(FSEL_OUTP);
    D5_PIN->WriteBool(false); D5_PIN->SetPinMode(FSEL_OUTP);
    D6_PIN->WriteBool(false); D6_PIN->SetPinMode(FSEL_OUTP);
    D7_PIN->WriteBool(false); D7_PIN->SetPinMode(FSEL_OUTP);
  }

  Utils::delay_ms(35);
  m_isInit = true;

  sendInitSequence();

  return err;
}

void LCD::sendInitSequence()
{
  if (m_bits == 4)
  {
    int func = LCD_FUNC | LCD_FUNC_DL ;			// Set 8-bit mode 3 times
    sendNibble(func >> 4); Utils::delay_ms(35) ;
    sendNibble(func >> 4); Utils::delay_ms (35) ;
    sendNibble(func >> 4); Utils::delay_ms (35) ;
    func = LCD_FUNC ;					// 4th set: 4-bit mode
    sendNibble(func >> 4); Utils::delay_ms (35) ;
  }
  else
  {
    int func = LCD_FUNC | LCD_FUNC_DL ;
    sendByte(false, func); Utils::delay_ms(35);
    sendByte(false, func); Utils::delay_ms(35);
    sendByte(false, func); Utils::delay_ms(35);
  }

  if (m_rows > 1)
  {
    int func = LCD_FUNC | LCD_FUNC_DL | LCD_FUNC_N;
    sendByte(false, func); Utils::delay_ms(35);
  }

  // Rest of the initialisation sequence

  display(true);
  cursor(false);
  blink(false);
  clear();

  sendByte(false, LCD_ENTRY   | LCD_ENTRY_ID);
  sendByte(false, LCD_CDSHIFT | LCD_CDSHIFT_RL);
}

// goto 0;0
ErrorCode LCD::home()
{
  if (! m_isInit) return E_BLOCK_NOT_INIT;

  sendByte(false, LCD_HOME);
  m_cx = m_cy = 0;

  Utils::delay_ms(5);

  return E_OK;
}

// clear display
ErrorCode LCD::clear()
{
 if (! m_isInit) return E_BLOCK_NOT_INIT;

  sendByte(false, LCD_CLEAR);
  sendByte(false, LCD_HOME);
  m_cx = m_cy = 0;

  Utils::delay_ms(5);

  return E_OK;
}

// switch on/off
ErrorCode LCD::display(bool on)
{
  if (! m_isInit) return E_BLOCK_NOT_INIT;

  if (on) m_control |= LCD_DISPLAY_CTRL;
  else m_control &= ~LCD_DISPLAY_CTRL;

  sendByte(false, LCD_CTRL | m_control);
  Utils::delay_ms(5);
 
  return E_OK;
}

// switch cursor on/off
ErrorCode LCD::cursor(bool on)
{
  if (! m_isInit) return E_BLOCK_NOT_INIT;

  if (on) m_control |= LCD_CURSOR_CTRL;
  else m_control &= ~LCD_CURSOR_CTRL;

  sendByte(false, LCD_CTRL | m_control);
  Utils::delay_ms(5);
 
  return E_OK;
}

// switch on blinking of cursor
ErrorCode LCD::blink(bool on)
{
  if (! m_isInit) return E_BLOCK_NOT_INIT;

  if (on) m_control |= LCD_BLINK_CTRL;
  else m_control &= ~LCD_BLINK_CTRL;

  sendByte(false, LCD_CTRL | m_control);
  Utils::delay_ms(5);
 
  return E_OK;
}

// send command
ErrorCode LCD::command(unsigned char cmd)
{
  if (! m_isInit) return E_BLOCK_NOT_INIT;

  sendByte(false, cmd);
  Utils::delay_ms(5);

  return E_OK;
}

// goto position, 0,0 is left top
ErrorCode LCD::position(int x, int y)
{
  if (! m_isInit) return E_BLOCK_NOT_INIT;

  if (x > m_cols || x < 0) return E_BLOCK_INVALID_PARAMS;
  if (y > m_rows || y < 0) return E_BLOCK_INVALID_PARAMS;

  int rowOffset[] = {0x00, 0x40, 0x14, 0x54};

  if (m_rows > 3) m_rows = 3;

  sendByte(false, x + (LCD_DGRAM | rowOffset [y]));
  Utils::delay_ms(5);

  m_cx = x; 
  m_cy = y;

  return E_OK;
}

// define character map
ErrorCode LCD::defineChar(int index, unsigned char data[8])
{
  if (! m_isInit) return E_BLOCK_NOT_INIT;

  sendByte(false, LCD_CGRAM | ((index & 7) << 3));

  for (int i=0; i<8; i++)
    sendByte(true, data[i]);

  return E_OK;
}

// print one char at current location
ErrorCode LCD::putChar(unsigned char data)
{
  if (! m_isInit) return E_BLOCK_NOT_INIT;

  sendByte(true, data);

  int x = m_cx; 
  int y = m_cy;

  ErrorCode err = E_OK;

  // advance cursor
  if (++x == m_cols)
  {
    x = 0 ;
    if (++y == m_rows)
      y = 0 ;
    
    err = position(x,y) ;
  }

  return err;
}

// print string at current position
ErrorCode LCD::putString(const std::string &data)
{
  if (! m_isInit) return E_BLOCK_NOT_INIT;
  
  ErrorCode err = E_OK;

  for (auto it=data.begin(); it != data.end(); ++it)
  {
    err = putChar(*it);
    if (err != E_OK) break;
  }

  return err;
}

// ------------------------------------------------------------------------
void LCD::strobe()
{
  if (! STRB_PIN) return;

  STRB_PIN->WriteBool(true);
  Utils::delay_ms(1);
  STRB_PIN->WriteBool(false);
  Utils::delay_ms(1);
}

void LCD::sendByte(bool RS, unsigned char data)
{
  if (! m_isInit) return;

  unsigned char localData = data ;
  unsigned char d4 ;

  RS_PIN->WriteBool(RS);

  if (m_bits == 4)
  {
    d4 = (localData >> 4) & 0x0F;
 
    D0_PIN->WriteBool(d4 & 1);
    d4 >>= 1;
    D1_PIN->WriteBool(d4 & 1);
    d4 >>= 1;
    D2_PIN->WriteBool(d4 & 1);
    d4 >>= 1;
    D3_PIN->WriteBool(d4 & 1);
    
    strobe () ;

    d4 = localData & 0x0F ;
    D0_PIN->WriteBool(d4 & 1);
    d4 >>= 1;
    D1_PIN->WriteBool(d4 & 1);
    d4 >>= 1;
    D2_PIN->WriteBool(d4 & 1);
    d4 >>= 1;
    D3_PIN->WriteBool(d4 & 1);

    strobe () ;
  }
  else
  {
    D0_PIN->WriteBool(localData & 1);
    localData >>= 1;
    D1_PIN->WriteBool(localData & 1);
    localData >>= 1;
    D2_PIN->WriteBool(localData & 1);
    localData >>= 1;
    D3_PIN->WriteBool(localData & 1);
    localData >>= 1;
    D4_PIN->WriteBool(localData & 1);
    localData >>= 1;
    D5_PIN->WriteBool(localData & 1);
    localData >>= 1;
    D6_PIN->WriteBool(localData & 1);
    localData >>= 1;
    D7_PIN->WriteBool(localData & 1);

    strobe () ;
  }

  Utils::delay_ms(2);
}

// we use this onlz during init, so alwazs set RS to false
void LCD::sendNibble(unsigned char data)
{
  if (! m_isInit) return;

  unsigned char localData = data ;

  RS_PIN->WriteBool(false);

  D0_PIN->WriteBool(localData & 1);
  localData >>= 1;
  D1_PIN->WriteBool(localData & 1);
  localData >>= 1;
  D2_PIN->WriteBool(localData & 1);
  localData >>= 1;
  D3_PIN->WriteBool(localData & 1);
  
  strobe () ;
}
