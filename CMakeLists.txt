cmake_minimum_required(VERSION 3.0)
project(uv)

if (UNIX)
install (FILES include/uv DESTINATION 		/usr/local/include)
install (DIRECTORY include/uv++ DESTINATION 	/usr/local/include)
endif (UNIX)

add_subdirectory(samples)
