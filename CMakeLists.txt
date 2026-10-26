cmake_minimum_required(VERSION 3.18)
project(aa)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

find_package(Lua 5.1 EXACT REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(SFML 2.6 COMPONENTS graphics window system audio REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE  ${LUA_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${LUA_LIBRARIES} SQLite3::SQLite3 sfml-graphics sfml-window sfml-system sfml-audio)
