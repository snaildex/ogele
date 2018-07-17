cmake_minimum_required(VERSION 3.9)
project(ogele)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE GLEW_SOURCE core/GL/*)
file(GLOB_RECURSE GLFW_SOURCE core/GLFW/*)
file(GLOB_RECURSE TINYXML_SOURCE core/tinyxml2/*)
file(GLOB_RECURSE IMGUI_SOURCE core/imgui/*)
file(GLOB_RECURSE GLM_SOURCE core/glm/*)
file(GLOB_RECURSE OGELECORE_SOURCE core/ogele.cpp core/ogele.h)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dummy)

add_executable(ogele
        dummy/OgeleDummy.cpp
        ${OGELECORE_SOURCE}
        ${GLEW_SOURCE}
        ${GLFW_SOURCE}
        ${GLM_SOURCE}
        ${IMGUI_SOURCE}
        ${TINYXML_SOURCE}
        core/stb_image.h
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