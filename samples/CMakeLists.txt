cmake_minimum_required(VERSION 3.0)
add_compile_options(-std=c++11)

include_directories(../include/)

set(DEPS_LIBRARIES uv pthread rt)

add_executable(timer timer.cpp)
target_link_libraries(timer ${DEPS_LIBRARIES})

add_executable(daytimeclient daytimeclient.cpp)
target_link_libraries(daytimeclient ${DEPS_LIBRARIES})

add_executable(interrupt_signal interrupt_signal.cpp)
target_link_libraries(interrupt_signal ${DEPS_LIBRARIES})
