cmake_minimum_required(VERSION 3.5)
project(TGOpenGLEngine)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(assimp REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRC_LIST)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

add_executable(main.out  ${SRC_LIST})
target_link_libraries(main.out GLEW::GLEW glfw glm::glm assimp::assimp ${OPENGL_LIBRARIES})