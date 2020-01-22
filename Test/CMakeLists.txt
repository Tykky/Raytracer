project(Googletest)

add_subdirectory(lib/googletest)

set(SRC ${CMAKE_SOURCE_DIR}/Source)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${SRC})

add_executable(gtest_run dummytest.cpp)
target_link_libraries(gtest_run gtest gtest_main)
target_link_libraries(gtest_run ${SRC}/Dummyclass.cpp)