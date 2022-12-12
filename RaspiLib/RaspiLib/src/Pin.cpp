#include "Pin.h"

using namespace PiLib;

Pin::Pin(RaspiPinLabel whichOne)
{
  m_pin = whichOne;
  m_GPIONumber = findGPIOForPin(whichOne);

  readSettings();
}

Pin::~Pin()
{}

// translate RaspiPin to GPIO
int Pin::findGPIOForPin(RaspiPinLabel whichOne)
{
    // /* RPi B+ J8 header, also RPi 2 40 pin GPIO header */
    // RPI_BPLUS_GPIO_J8_03     =  2,  /*!< B+, Pin J8-03 */
    // RPI_BPLUS_GPIO_J8_05     =  3,  /*!< B+, Pin J8-05 */
    // RPI_BPLUS_GPIO_J8_07     =  4,  /*!< B+, Pin J8-07 */
    // RPI_BPLUS_GPIO_J8_08     = 14,  /*!< B+, Pin J8-08, defaults to alt function 0 UART0_TXD */
    // RPI_BPLUS_GPIO_J8_10     = 15,  /*!< B+, Pin J8-10, defaults to alt function 0 UART0_RXD */
    // RPI_BPLUS_GPIO_J8_11     = 17,  /*!< B+, Pin J8-11 */
    // RPI_BPLUS_GPIO_J8_12     = 18,  /*!< B+, Pin J8-12, can be PWM channel 0 in ALT FUN 5 */
    // RPI_BPLUS_GPIO_J8_13     = 27,  /*!< B+, Pin J8-13 */
    // RPI_BPLUS_GPIO_J8_15     = 22,  /*!< B+, Pin J8-15 */
    // RPI_BPLUS_GPIO_J8_16     = 23,  /*!< B+, Pin J8-16 */
    // RPI_BPLUS_GPIO_J8_18     = 24,  /*!< B+, Pin J8-18 */
    // RPI_BPLUS_GPIO_J8_19     = 10,  /*!< B+, Pin J8-19, MOSI when SPI0 in use */
    // RPI_BPLUS_GPIO_J8_21     =  9,  /*!< B+, Pin J8-21, MISO when SPI0 in use */
    // RPI_BPLUS_GPIO_J8_22     = 25,  /*!< B+, Pin J8-22 */
    // RPI_BPLUS_GPIO_J8_23     = 11,  /*!< B+, Pin J8-23, CLK when SPI0 in use */
    // RPI_BPLUS_GPIO_J8_24     =  8,  /*!< B+, Pin J8-24, CE0 when SPI0 in use */
    // RPI_BPLUS_GPIO_J8_26     =  7,  /*!< B+, Pin J8-26, CE1 when SPI0 in use */
    // RPI_BPLUS_GPIO_J8_29     =  5,  /*!< B+, Pin J8-29,  */
    // RPI_BPLUS_GPIO_J8_31     =  6,  /*!< B+, Pin J8-31,  */
    // RPI_BPLUS_GPIO_J8_32     = 12,  /*!< B+, Pin J8-32,  */
    // RPI_BPLUS_GPIO_J8_33     = 13,  /*!< B+, Pin J8-33,  */
    // RPI_BPLUS_GPIO_J8_35     = 19,  /*!< B+, Pin J8-35, can be PWM channel 1 in ALT FUN 5 */
    // RPI_BPLUS_GPIO_J8_36     = 16,  /*!< B+, Pin J8-36,  */
    // RPI_BPLUS_GPIO_J8_37     = 26,  /*!< B+, Pin J8-37,  */
    // RPI_BPLUS_GPIO_J8_38     = 20,  /*!< B+, Pin J8-38,  */
    // RPI_BPLUS_GPIO_J8_40     = 21   /*!< B+, Pin J8-40,  */

  static int PinMap[] = 
  {
    GPIO_DUMMY,  // PIN_UNDEFINED = 0,
    GPIO_DUMMY,  // PIN1_3v3 = 1,
    GPIO_DUMMY,  // PIN2_5v,
     2,          // PIN3_GPIO2_SDA,         // I2C SDA
    GPIO_DUMMY,  // PIN4_5v,
     3,          // PIN5_GPIO3_SCL,         // I2C SCL
    GPIO_DUMMY,  // PIN6_GND,
     4,          // PIN7_GPIO4_GPCLK0,      // one wire protocol
    14,          // PIN8_GPIO14_TXD,
    GPIO_DUMMY,  // PIN9_GND,
    15,          // PIN10_GPIO15_RXD,
    17,          // PIN11_GPIO17,
    18,          // PIN12_GPIO18_PCMC,
    27,          // PIN13_GPIO27_PCMD,
    GPIO_DUMMY,  // PIN14_GND,
    22,          // PIN15_GPIO22,
    23,          // PIN16_GPIO23,
    GPIO_DUMMY,  // PIN17_3v3,
    24,          // PIN18_GPIO24,
    10,          // PIN19_GPIO10_MOSI,
    GPIO_DUMMY,  // PIN20_GND,
     9,          // PIN21_GPIO9_MISO,
    25,          // PIN22_GPIO25,
    11,          // PIN23_GPIO11_SCKL,
     8,          // PIN24_GPIO8_CE0,
    GPIO_DUMMY,  // PIN25_GND,
     7,          // PIN26_GPIO7_CE1,
     0,          // PIN27_GPIO0_IDSD,
     1,          // PIN28_GPIO1_IDSC,
     5,          // PIN29_GPIO5,
    GPIO_DUMMY,  // PIN30_GND,
     6,          // PIN31_GPIO6,
    12,          // PIN32_GPIO12,
    13,          // PIN33_GPIO13,
    GPIO_DUMMY,  // PIN34_GND,
    19,          // PIN35_GPIO19_MISO,
    16,          // PIN36_GPIO16,
    26,          // PIN37_GPIO26,
    20,          // PIN38_GPIO20_MOSI,
    GPIO_DUMMY,  // PIN39_GND,
    21,          // PIN40_GPIO21_SCLK,

    GPIO_DUMMY,  // PIN_SENTINEL
  };

  int ret = GPIO_DUMMY;

  if (whichOne < PIN_SENTINEL)
    ret = PinMap[whichOne];

  return ret;
}

// init pin with current register settings
void Pin::readSettings()
{

}


void Pin::SetPinMode(PinFunctionSelect mode)
{
  if (m_GPIONumber < 0) return;

  bcm2835_gpio_fsel(m_GPIONumber, mode);
  m_mode = mode;
}

void Pin::SetPullUpDnConfig(PinPullUpDownSelect pud)
{
  if (m_GPIONumber < 0) return;

  bcm2835_gpio_set_pud(m_GPIONumber, pud);
  m_pud = pud;
}

bool Pin::ReadBool()
{
  if (m_GPIONumber < 0) return false;
  
  uint8_t state = bcm2835_gpio_lev(m_GPIONumber);
  return (state == HIGH);
}

void Pin::WriteBool(bool value)
{
  if (m_GPIONumber < 0) return;

  bcm2835_gpio_write(m_GPIONumber, (value) ? HIGH : LOW);
}
