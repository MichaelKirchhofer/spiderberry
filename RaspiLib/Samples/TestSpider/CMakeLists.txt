add_executable(TestSpider

main.cpp

)

target_link_libraries(TestSpider PUBLIC ${EXTRA_LIBS} ${OS_LIBS})

target_include_directories(TestSpider PUBLIC
                           ${PROJECT_BINARY_DIR} 
                           ${EXTRA_INCLUDES}
                           )                          

