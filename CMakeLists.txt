cmake_minimum_required(VERSION 3.0)

if (UNIX)
install (FILES include/uv DESTINATION 		/usr/local/include)
install (DIRECTORY include/uv++ DESTINATION 	/usr/local/include)
endif (UNIX)
