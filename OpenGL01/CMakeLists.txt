cmake_minimum_required(VERSION 3.5)
set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

project(TGOpenGLEngine)
set(exename OpenGL01)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(assimp REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)

# get_cmake_property(_variableNames VARIABLES)
# foreach (_variableName ${_variableNames})
#     message(STATUS ">> ${_variableName}=${${_variableName}}")
# endforeach()

# set(CMAKE_C_STANDARD 17)
# set(CMAKE_CXX_STANDARD 17)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRC_LIST)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

add_executable(${exename}  ${SRC_LIST})
target_link_libraries(${exename} glfw glm::glm assimp::assimp ${GLEW_LIBRARY} ${OPENGL_LIBRARIES})