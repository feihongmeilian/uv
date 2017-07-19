cmake_minimum_required(VERSION 3.0)
add_compile_options(-std=c++11)

add_executable(timer timer.cpp)

target_link_libraries(timer uv.a pthread)
