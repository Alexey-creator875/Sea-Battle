cmake_minimum_required(VERSION 3.23)

project(SeaBattle)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SeaBattleExecutor.cpp
)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/SeaBattleExecutor.h
)

find_package(SFML REQUIRED COMPONENTS Graphics)
find_package(SFML REQUIRED COMPONENTS Window)

add_executable(SeaBattle ${SOURCES} ${HEADERS})

target_link_libraries(SeaBattle PRIVATE SFML::Graphics)

if(ENABLE_DEBUG)
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -g)
endif()