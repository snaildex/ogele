cmake_minimum_required(VERSION 3.9)
project(ogele)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dummy)

file(GLOB_RECURSE OGELECORE_SOURCE core/*)

add_executable(ogele
        dummy/OgeleDummy.cpp
        ${OGELECORE_SOURCE}
        )

find_package(OpenGL REQUIRED)
find_library(GLFW_LIB
        NAMES libglfw3
        HINTS "lib"
        )
find_library(STDFS
        NAMES libstdc++fs
        HINTS "lib"
        )
include_directories( ${OPENGL_INCLUDE_DIRS}  )
target_link_libraries(ogele ${OPENGL_gl_LIBRARY} )
target_link_libraries(ogele ${GLFW_LIB} )
target_link_libraries(ogele ${STDFS} )