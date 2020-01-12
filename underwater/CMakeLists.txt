cmake_minimum_required(VERSION 3.9)
project(underwater)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE DUMMY_SOURCE 
source/* 
resources/*
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Sources" FILES ${DUMMY_SOURCE})

add_executable(underwater ${DUMMY_SOURCE})

target_link_libraries(underwater core)

set_target_properties(underwater PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources")