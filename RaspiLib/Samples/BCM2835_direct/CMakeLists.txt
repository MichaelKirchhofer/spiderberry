add_executable(BCM2835Test

src/main.cpp
src/adctest.cpp 
)

target_link_libraries(BCM2835Test PUBLIC ${EXTRA_LIBS} ${OS_LIBS})

target_include_directories(BCM2835Test PUBLIC
                           ${PROJECT_BINARY_DIR} 
                           ${EXTRA_INCLUDES}
						   ${CMAKE_CURRENT_SOURCE_DIR}/inc
                           )                          

