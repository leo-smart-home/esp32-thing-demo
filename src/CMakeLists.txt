FILE(GLOB_RECURSE app_sources "${CMAKE_SOURCE_DIR}/src/app2/*.c")

set(sources ${app_sources})

idf_component_register(
    SRCS ${sources} 
    INCLUDE_DIRS "app2" 
)
