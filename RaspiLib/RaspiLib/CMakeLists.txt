add_library(RaspiLib

src/RaspiLib.cpp
src/Pin.cpp
src/RaspiBoard.cpp
src/Utils.cpp
src/I2C.cpp

src/Quaternion.cpp
src/Vector3D.cpp
src/KalmanFilter.cpp

)

target_include_directories(RaspiLib PUBLIC
                           ${PROJECT_SOURCE_DIR}/BCM2835
                           ${PROJECT_SOURCE_DIR}/smbus/include
						               ${CMAKE_CURRENT_SOURCE_DIR}/inc 
                          )
