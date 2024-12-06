# This file was automatically generated for projects
# without default 'CMakeLists.txt' file.

FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/src/app/*.*)
# FILE(GLOB_RECURSE mcu_sources ${CMAKE_SOURCE_DIR}/src/mcu/*.*)

idf_component_register(SRCS ${app_sources})
# idf_component_register(SRCS ${mcu_sources})
