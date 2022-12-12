add_executable(i2c-test 

  src/main.cpp
)

target_link_libraries(i2c-test PUBLIC ${EXTRA_LIBS} ${OS_LIBS})

target_include_directories(i2c-test PUBLIC
                           ${PROJECT_BINARY_DIR} 
                           ${EXTRA_INCLUDES}
            						   ${CMAKE_CURRENT_SOURCE_DIR}/inc
                           )                          
