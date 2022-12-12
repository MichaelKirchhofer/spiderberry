add_library(RaspiBlocks

src/PCF8591.cpp
src/PCF8563.cpp
src/LCD_I2C.cpp
src/UltraSound.cpp
src/MPU6050.cpp

src/lcd.cpp
src/PCA9685.cpp
src/ADS7830.cpp

src/HTS221.cpp
src/LPS25H.cpp
)

target_include_directories(RaspiBlocks PUBLIC
                           ${PROJECT_SOURCE_DIR}/BCM2835
						   ${PROJECT_SOURCE_DIR}/RaspiLib/inc
						   ${CMAKE_CURRENT_SOURCE_DIR}/inc
                          )
