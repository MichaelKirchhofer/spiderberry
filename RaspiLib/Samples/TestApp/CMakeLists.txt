add_executable(TestApp

src/main.cpp
src/blinker.cpp
src/lcdSample.cpp
src/adcSample.cpp 
src/RTCTest.cpp
src/USound.cpp
src/GyroTest.cpp
src/PCA9685test.cpp
src/sensehat.cpp
)

target_link_libraries(TestApp PUBLIC ${EXTRA_LIBS} ${OS_LIBS})

target_include_directories(TestApp PUBLIC
                           ${PROJECT_BINARY_DIR} 
                           ${EXTRA_INCLUDES}
						   ${CMAKE_CURRENT_SOURCE_DIR}/inc
                           )                          

