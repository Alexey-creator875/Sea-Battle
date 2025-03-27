cmake_minimum_required(VERSION 3.23)

set(project "SeaBattle")
project(${project})

set(CMAKE_CXX_STANDARD 20)

set(${project}_SOURCES
        main.cpp
        SeaBattleExecutor.cpp)

set(${project}_HEADERS
        SeaBattleExecutor.h)

set(${project}_SOURCE_LIST
        ${${project}_SOURCES}
        ${${project}_HEADERS})

add_executable(${project}
        ${${project}_SOURCE_LIST})

#target_link_libraries(${project} Fraction)

if(ENABLE_DEBUG)
    target_compile_options(${project} PRIVATE -g)
endif()