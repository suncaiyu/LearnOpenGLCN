CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../includes)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

aux_source_directory(. ALL_SRCS)

add_executable(day_8_1 ${ALL_SRCS} ../glad.c ../stb_image.cpp)
target_link_libraries(day_8_1
    ${PROJECT_SOURCE_DIR}/lib/libImguiLib.a
	${PROJECT_SOURCE_DIR}/lib/mingw/libglfw3dll.a)	
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/day_8_1)

add_custom_command(TARGET day_8_1 
POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/lib/mingw/glfw3.dll ${PROJECT_SOURCE_DIR}/bin/day_8_1
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/container2.png ${PROJECT_SOURCE_DIR}/bin/day_8_1
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/8_1cube.vs ${PROJECT_SOURCE_DIR}/bin/day_8_1
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/8_1cube.fs ${PROJECT_SOURCE_DIR}/bin/day_8_1
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/container2_specular.png ${PROJECT_SOURCE_DIR}/bin/day_8_1
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/lightint.vs ${PROJECT_SOURCE_DIR}/bin/day_8_1
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/lighting.fs ${PROJECT_SOURCE_DIR}/bin/day_8_1)