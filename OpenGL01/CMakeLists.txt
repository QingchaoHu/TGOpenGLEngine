cmake_minimum_required(VERSION 3.5)
project(TGOpenGLEngine)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(assimp REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRC_LIST)
add_executable(main.out  ${SRC_LIST})
target_link_libraries(main.out GLEW glfw glm assimp ${OPENGL_LIBRARIES})