add_executable (SpiderTest

  main.cpp
  LegTest.cpp
  ## stub implementation
  DummyCallback.cpp

  TransformationTest.cpp
  WalkTest.cpp

)

set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Catch2)
##add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

target_include_directories(SpiderTest PRIVATE
  ${CATCH_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../RaspiLib/inc
  ${CMAKE_CURRENT_SOURCE_DIR}/../RaspiBlocks/inc
  ${CMAKE_CURRENT_SOURCE_DIR}/../Spider/inc
)

target_link_libraries(SpiderTest PUBLIC ${EXTRA_LIBS} ${OS_LIBS})

enable_testing()

add_test(
  NAME SpiderTest
  COMMAND SpiderTest
)
