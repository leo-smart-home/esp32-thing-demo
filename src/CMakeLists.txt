



# This file was automatically generated for projects
# without default 'CMakeLists.txt' file.

# FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/src/app/*.*)
# idf_component_register(SRCS ${app_sources})



FILE(GLOB_RECURSE app_sources "${CMAKE_SOURCE_DIR}/src/app/*.c")
FILE(GLOB_RECURSE core_sources "${CMAKE_SOURCE_DIR}/src/core/*.c")
FILE(GLOB_RECURSE srv_sources "${CMAKE_SOURCE_DIR}/src/srv/*.c")

set(sources ${app_sources} ${core_sources} ${srv_sources})

idf_component_register(SRCS ${sources} 
                       INCLUDE_DIRS "app" "core" "srv")

