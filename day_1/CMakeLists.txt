#Cmake最低版本要求
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../includes)

aux_source_directory(. ALL_SRCS)

add_executable(day_1 ${ALL_SRCS} ../glad.c)
target_link_libraries(day_1
	${PROJECT_SOURCE_DIR}/lib/mingw/libglfw3dll.a)	
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/day_1)

#  g++ day_1.cpp ..\glad.c -o day_1 -I ..\includes -L ..\lib\mingw -l glfw3