cmake_minimum_required(VERSION 3.9)
project(ogele)
set(CMAKE_SUPPRESS_REGENERATION true)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT dummy)

add_subdirectory(core)
add_subdirectory(dummy)