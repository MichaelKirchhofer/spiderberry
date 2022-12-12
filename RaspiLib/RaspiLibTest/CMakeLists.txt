add_executable (RaspiLibTest

  main.cpp
  BCDTest.cpp

)

set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Catch2)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

target_include_directories(RaspiLibTest PRIVATE
  ${CATCH_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../RaspiLib/inc
  ${CMAKE_CURRENT_SOURCE_DIR}/../RaspiBlocks/inc
)

target_link_libraries(RaspiLibTest PUBLIC ${EXTRA_LIBS} ${OS_LIBS})

enable_testing()

add_test(
  NAME RaspiLibTest
  COMMAND RaspiLibTest
)
