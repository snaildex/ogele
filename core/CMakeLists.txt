cmake_minimum_required(VERSION 3.9)
project(core)

file(GLOB_RECURSE CORE_SOURCE source/*)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Sources" FILES ${CORE_SOURCE})

add_library(core STATIC ${CORE_SOURCE})

if (MSVC)
   set_target_properties(core PROPERTIES COMPILE_FLAGS "/Yustdafx.h")
   set_source_files_properties(source/stdafx.cpp PROPERTIES COMPILE_FLAGS "/Ycstdafx.h")
endif(MSVC)

target_include_directories (core PUBLIC source)

find_package(OpenGL REQUIRED)
target_include_directories(core PRIVATE ${OPENGL_INCLUDE_DIRS})
target_link_libraries(core ${OPENGL_gl_LIBRARY})
target_link_libraries(core libglew_static glfw assimp)

set_target_properties(core PROPERTIES VS_GLOBAL_ROOTNAMESPACE "ogele")