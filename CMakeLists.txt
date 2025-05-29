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

add_subdirectory(GameClasses)
add_executable(SeaBattle ${SOURCES} ${HEADERS})

target_link_libraries(SeaBattle PRIVATE SFML::Graphics)
target_link_libraries(${project} GameClasses)

if(ENABLE_DEBUG)
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -g)
endif()