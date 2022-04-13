CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

include_directories(${CMAKE_SOURCE_DIR}/includes)

aux_source_directory(. ALL_SRCS)

add_executable(day_2 ${ALL_SRCS} ../glad.c)
target_link_libraries(day_2
	${PROJECT_SOURCE_DIR}/lib/mingw/libglfw3dll.a)	
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/day_2)
add_custom_command(TARGET day_2
POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/lib/mingw/glfw3.dll ${PROJECT_SOURCE_DIR}/bin/day_2)