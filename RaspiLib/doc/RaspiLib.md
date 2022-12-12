# RaspiLib Architecture Document

The RaspiLib is the library to be used for the Embedded Software exercises. The library implements all the hardware related control functions. Your software is expected to run on top of the Spider library. In case any implementation is missing you are free to extend the library at any level.

This document explains the design rationale for the RaspiLib library.

## The basic structure

RaspiLib consists of several layers of code. The application usually uses the top layer, where several communication modules are available as objects.

```plantuml
package bcm2835
package RaspiLib 
package RaspiBlocks 
package Spider

RaspiLib -left-> bcm2835: basics
RaspiBlocks -left-> RaspiLib: IO components
Spider -left-> RaspiBlocks: implementation
```

### bcm2835

The bcm2835 layer is a basic Raspberry Pi abstraction, which contains all the parameters of the CPU datasheet. This package is available at [this link](http://www.airspayce.com/mikem/bcm2835/).

This is a C library for Raspberry Pi (RPi). It provides access to GPIO and other IO functions on the Broadcom BCM 2835 chip, as used in  he RaspberryPi, allowing access to the GPIO pins on the 26 pin IDE plug on the RPi board so you can control and interface with  various external devices.

It provides functions for reading digital inputs and setting digital outputs, using SPI and I2C, and for accessing the system timers. Pin event detection is supported by polling (interrupts are not supported).

### RaspiLib

The RaspiLib provides an abstraction layer to the Raspberry Pi board. Additionally, it provides a few utility classes which can be of use for further high level applications.

#### RaspiLib singleton

The main access point is the RaspiLib object. This is a singleton representing the Raspberry Pi instance in use. Due to access rights this singleton must be instantiated as root. Otherwise, it will fail.

```plantuml
class RaspiLib {
  +{static} bool Startup();
  +{static} bool Shutdown();

  +{static} void LibVersion(unsigned &major, unsigned &minor);

  +{static} RaspiLib &GetInstance();

  IBoard &GetBoard();
  IPin &GetPin(RaspiPinLabel label);
  II2c &GetI2c();
}
```

The library is started by calling the Startup() method. After a successful startup, the RaspiLib instance shall be retrieved and used for the rest of the program.

Several interfaces are in place to provide access to the board resources. As the base library does not support interrupts, only polling access is possible.

#### IBoard interface

The IBoard interface provides information about the Raspberry Pi board in place. This information data is represented by a BoardInfo structure, which is filled out upon request.

#### IPin interface

The IPin interface provides constants and access functions for the single pins supported by the Raspberry Pi Extension Connector. The numbering as well as the main role of the pin are included in the constant name. Pin mode and pullup/pulldown configuration can be set as well.

A single pin directly supports the setting and retrieving of the pin state. Depending on the alternate configurations (e.g., I2C and SPI), some pins may not be available directly, since they are captured by the higher level communication implementations.

#### II2c interface

The II2c interface provides access to the BCM2835 I2C block. By default, the block is initialized once the II2C interface is requested. There are basic functions to set the baud rate, and to read and write data to a specific I2C slave. For reading and writing registers in the chip, there are two more functions which allow dedicate addressing of specific registers in the slave.

One special function is provided to scan the I2C bus for slave addresses. All slaves found are returned in a byte vector, where each active slave found is listed.

#### IISpi interface

__TBD__

#### Error Handling

All functions of the RaspiLib return error codes for unexpected problems. These error codes are available in the __ErrorCode__ enumeration.

#### spatial support classes

For the support of 3D calculations, the RaspiLib provides a __Vector3D__ and a __Quaternion__ implementation. These classes can be used when the gyroscope and accelerometer shall be used to track the 3D movements of the board. The __KalmanFilter__ implementation might come in handy when the data shall be treated before used for further calculations.

### RaspiBlocks

The RaspiBlocks folder contains a set of support classes, which usually represent different peripheral devices used by the Raspberry Pi.

The classes either use direct I/O, or they use one of the communication blocks. Usually, the implementation does this internally.

#### Direct I/O

The direct I/O components are the __UltraSound__ and the __lcd__ classes.

#### I2C components

The following I2C component implementations are currently available.

##### ADS7830

The ADS7830 is an 8 channel ADC with 8 bit resolution. By default it uses the I2C address 0x48.

It provides a Read() function, which takes a channel number 0 .. 7 and returns the 8 bit value.

```c++
ErrorCode ADS7830::Read(int channel, int &value);
```

##### LCD_I2C

This class is a special wrapper for the I2C adapter of the LCD display. The display adapter uses a PCF8574T 8 bit DIO device which operates at I2C address 0x27.

It provides functions for initializing, clearing and writing to the device.

```c++
ErrorCode Init();
ErrorCode Clear();

ErrorCode Write(int x, int y, std::string text);
```

##### PCA9685

The PCA9685 is a 16 channel PWM chip usually used for LED driving. It can also be used for driving Servos, which is the case in our application. Each Servo is connected to one output channel. The chip usually operates at I2C address 0x40.

It provides the following functions:

```c++
ErrorCode Init();
ErrorCode Reset();

// frequ -> prescale value for the chip
// 1 .. 3500 (datasheet 50 MHz / (4*4096))
// for Servo init with 50 Hz
ErrorCode SetPWMFrequency(uint16_t value);
ErrorCode GetPrescale(uint8_t &prescale);

// get the current settings for channel
ErrorCode GetPWM(uint8_t channel, uint16_t &on, uint16_t &off);
// set the modulation width, if on < 4096 the off value is calculated automatically
ErrorCode SetPWM(uint8_t channel, uint16_t on, uint16_t off = 0);

// set servo angle 0 .. 180 degrees
ErrorCode SetServoAngle(uint8_t channel, float angle_degrees);

ErrorCode SetPinMode(PCA9685_Mode2 val);

// low level access - debugging only
ErrorCode Write8(int reg, int value);    // write 1 byte
ErrorCode Read8(int reg, int &value);    // read one byte
```

The PWMFrequency is usually set to 50 Hz for driving the servos, and the __SetServoAngle()__ function can then be used, accepting an angle from 0° to 180°. The other functions allow direct access to other chip features, but are not used in the context of the exercises.

##### PCF8563

The PCF8563 is a Realtime Clock device which operates at I2C address 0x50 or 0x51.

It offers setting an getting of the current time. Time data is coded in a special TimeInfo structure, which is also part of this implementation.

```c++
ErrorCode SetTime(const TimeInfo &ti);
ErrorCode ReadTime(TimeInfo &ti);

static std::string ToString(const TimeInfo &ti);
```

##### PCF8591

##### MPU6050


### Spider

The __Spider__ library contains implementation classes which are used directly in the spider automation implementation. The __Spider__ class itself is the main container for all peripheral devices. Subclasses are instantiated for single control tasks, e.g., calculating the position of a leg, moving a leg, moving the head, etc.

The subclasses use callback functions in the main class to access the peripheral devices, thus allowing for replacing the spider callbacks in case of unit test scenarios.

```plantuml
class Spider {
  void work()
}

interface SpiderServos {
  ErrorCode SetServoAngle(std::uint8_t channel, float angle) 
}

Spider -|> SpiderServos

class Leg
class world

Spider --> Leg
Leg --> SpiderServos

Spider -- world

```

#### Leg

The leg coordinate system has its origin in the center of the spider mount point. The X axis runs in the direction of the spider leg outwards. The y axis runs perpendicular. And the z axis runs up. This means, the contact point of the spider leg is z = -14 mm.

The leg class supports leg coordinate to servo angle transformation. Since the servos on the left and right side are mounted differently, the transformation takes this into account automatically. During initialization the leg is positioned either on the right or on the left side of the spider.

Once the angles are calculated, the SpiderServos callback interface provided by the spider is used to set the servo angles. Management of the hardware is subject to the spider class, therefore the leg does not directly communicate with the I2C interfaces.

#### 
