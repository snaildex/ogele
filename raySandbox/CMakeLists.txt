cmake_minimum_required(VERSION 3.9)
project(raySandbox)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE DUMMY_SOURCE 
source/*.h 
source/*.cpp 
resources/*.vert
resources/*.tese
resources/*.tesc
resources/*.geom
resources/*.frag
resources/*.comp
resources/*.json
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Sources" FILES ${DUMMY_SOURCE})

add_executable(raySandbox ${DUMMY_SOURCE})

target_link_libraries(raySandbox core)

set_target_properties(raySandbox PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources")