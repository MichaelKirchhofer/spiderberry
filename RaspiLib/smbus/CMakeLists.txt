add_library(smbus 

  src/smbus.cpp
)

target_include_directories(smbus PUBLIC
            						   ${CMAKE_CURRENT_SOURCE_DIR}/include 
                           )                          
