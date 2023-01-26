cmake_minimum_required(VERSION 3.16)

project(RaspiLib)

#add_subdirectory(BCM2835)
#add_subdirectory(smbus)

#add_subdirectory(RaspiLib)
#add_subdirectory(Code)
#add_subdirectory(RaspiLib/RaspiBlocks)
#add_subdirectory(RaspiLib/Spider)
list(APPEND EXTRA_LIBS Spider RaspiBlocks RaspiLib BCM2835 smbus)
list(APPEND EXTRA_INCLUDES ${PROJECT_SOURCE_DIR}/Spider/inc ${PROJECT_SOURCE_DIR}/RaspiBlocks/inc ${PROJECT_SOURCE_DIR}/RaspiLib/inc ${PROJECT_SOURCE_DIR}/BCM2835 $(PROJECT_SOURCE_DIR)/smbus/include)

#add_subdirectory(RaspiLibTest)
#add_subdirectory(SpiderTest)

#add_subdirectory(Samples/TestApp)
#add_subdirectory(Samples/BCM2835_direct)
#add_subdirectory(Samples/TestSpider)
#add_subdirectory(Samples/I2C-Test)


add_library(spiderberry_library
RaspiLib/src/I2C_bcm2835.cpp
RaspiLib/src/I2C_smbus.cpp
RaspiLib/src/I2C.cpp
RaspiLib/src/KalmanFilter.cpp
RaspiLib/src/Pin.cpp
RaspiLib/src/Quaternion.cpp
RaspiLib/src/RaspiBoard.cpp
RaspiLib/src/RaspiLib.cpp
RaspiLib/src/Utils.cpp
RaspiLib/src/Vector3D.cpp

RaspiBlocks/src/ADS7830.cpp
RaspiBlocks/src/HTS221.cpp
RaspiBlocks/src/LCD_I2C.cpp
RaspiBlocks/src/lcd.cpp
RaspiBlocks/src/LPS25H.cpp
RaspiBlocks/src/MPU6050.cpp
RaspiBlocks/src/PCA9685.cpp
RaspiBlocks/src/PCF8563.cpp
RaspiBlocks/src/PCF8591.cpp
RaspiBlocks/src/UltraSound.cpp

Spider/src/Leg.cpp
Spider/src/Spider.cpp
Spider/src/world.cpp

smbus/src/smbus.cpp

BCM2835/bcm2835.c
BCM2835/bcm2835_wrapper.c
BCM2835/bcm2835_fake.c)

target_include_directories(spiderberry_library PUBLIC  Code/ RaspiLib/inc/ RaspiBlocks/inc Spider/inc smbus/inc/ BCM2835/)

add_executable(spiderberry_exe Code/src/main.cpp)
target_link_libraries(spiderberry_exe PRIVATE spiderberry_library)

#enable_testing()

#add_test(
 # NAME UTest
  #COMMAND RaspiLibTest
#)
