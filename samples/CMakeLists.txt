cmake_minimum_required(VERSION 3.0)
add_compile_options(-std=c++11)

add_executable(timer timer.cpp)
target_link_libraries(timer uv.a pthread)

add_executable(daytimeclient daytimeclient.cpp)
target_link_libraries(daytimeclient uv.a pthread)

add_executable(interrupt_signal interrupt_signal.cpp)
target_link_libraries(interrupt_signal uv.a pthread)