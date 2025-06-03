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
add_subdirectory(Battlefield)
add_subdirectory(CustomEntities)
add_subdirectory(Delay)


find_package(SFML REQUIRED COMPONENTS Graphics Window Audio)

add_executable(SeaBattle ${SOURCES} ${HEADERS})

target_link_libraries(SeaBattle GameClasses)
target_link_libraries(SeaBattle Battlefield)
target_link_libraries(SeaBattle CustomEntities)
target_link_libraries(SeaBattle Delay)
target_link_libraries(SeaBattle SFML::Graphics)
target_link_libraries(SeaBattle SFML::Window)
target_link_libraries(SeaBattle SFML::Audio)


if(ENABLE_DEBUG)
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -g)
endif()