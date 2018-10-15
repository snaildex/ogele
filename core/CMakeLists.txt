cmake_minimum_required(VERSION 3.9)
project(core)

file(GLOB_RECURSE CORE_SOURCE source/* include/*)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Sources" FILES ${CORE_SOURCE})

add_library(core STATIC ${CORE_SOURCE})

target_include_directories (core PUBLIC include)
		
find_package(OpenGL REQUIRED)
target_include_directories(core PRIVATE ${OPENGL_INCLUDE_DIRS})
target_link_libraries(core ${OPENGL_gl_LIBRARY})

target_include_directories(core PUBLIC 3d_party/include)
find_library(GLFW_LIB NAMES libglfw3 HINTS 3d_party/lib)
find_library(GLEW_LIB NAMES libglew32 HINTS 3d_party/lib)
target_link_libraries(core ${GLFW_LIB} ${GLEW_LIB})


set_target_properties(core PROPERTIES VS_GLOBAL_ROOTNAMESPACE "ogele")