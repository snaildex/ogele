cmake_minimum_required(VERSION 3.9)
project(dummy)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE DUMMY_SOURCE source/*)

add_executable(dummy ${DUMMY_SOURCE})

target_link_libraries(dummy core)

set_target_properties(dummy PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources")
