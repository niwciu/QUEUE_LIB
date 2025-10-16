cmake_minimum_required(VERSION 3.20)

add_library(queue_lib STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/queue.c    
)

set_target_properties(queue_lib PROPERTIES 
  OUTPUT_NAME "queue_lib"
  PREFIX ""
)

target_include_directories(queue_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

--- Coverage instrumentation (tylko jeśli build dla testów) ---
if(DEFINED UNIT_TESTS)
    message(STATUS "enabled coverage flag instrumentation")

    target_compile_options(queue_lib PRIVATE -fprofile-arcs -ftest-coverage)
    target_link_options(queue_lib PRIVATE -fprofile-arcs)
else()
  message(STATUS "Flags for generate coverage reguired files for the lib not enabled")
endif()
