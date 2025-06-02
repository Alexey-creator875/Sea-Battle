cmake_minimum_required(VERSION 3.23)

set(project "CustomEntities")
project(${project})

set(CMAKE_CXX_STANDARD 17)

set(${project}_SOURCES
        Button.cpp)

set(${project}_HEADERS
        Button.h)

set(${project}_SOURCE_LIST
        ${${project}_SOURCES}
        ${${project}_HEADERS})

find_package(SFML REQUIRED COMPONENTS Graphics)

add_library(${project}
        STATIC
        ${${project}_SOURCE_LIST})
        
target_link_libraries(${project} SFML::Graphics)
