cmake_minimum_required(VERSION 3.23)

project(chr C)

option(BUILD_CLASSINFO "Build the classinfo program" ON)

add_subdirectory(deps/bin-reader)

add_library(chr STATIC src/class.c)

target_link_libraries(chr bin-reader)

target_include_directories(chr PRIVATE src include/chr)
target_include_directories(chr PUBLIC deps/bin-reader/include)

if(BUILD_CLASSINFO)
    add_executable(classinfo src/main.c)

    target_link_libraries(classinfo chr)
    target_include_directories(classinfo PRIVATE include)
endif()