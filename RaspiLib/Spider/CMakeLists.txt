add_library(Spider

src/Spider.cpp
src/Leg.cpp
src/world.cpp

)

target_include_directories(Spider PUBLIC
                           ${PROJECT_SOURCE_DIR}/BCM2835
						   ${PROJECT_SOURCE_DIR}/RaspiLib/inc
						   ${PROJECT_SOURCE_DIR}/RaspiBlocks/inc
						   ${CMAKE_CURRENT_SOURCE_DIR}/inc
                          )
