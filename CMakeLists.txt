cmake_minimum_required(VERSION 3.10)

project(release VERSION 1.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
   set(CMAKE_C_COMPILER "clang")
   set(CMAKE_CXX_COMPILER "clang++")
endif()

# Include the directories.
include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*/*.cpp)

# Add the executable.
add_executable(${PROJECT_NAME} ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

# Copy files
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/sounds $<TARGET_FILE_DIR:${PROJECT_NAME}>/sounds
   COMMENT "Copying the sounds folder...")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/script $<TARGET_FILE_DIR:${PROJECT_NAME}>/script
   COMMENT "Copying the script folder...")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/editor $<TARGET_FILE_DIR:${PROJECT_NAME}>/editor
   COMMENT "Copying the editor folder...")

file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/build/data)

# Link libraries.
find_package(SFML 2.6 COMPONENTS graphics window system audio REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE lua5.1 sqlite3 sfml-graphics sfml-window sfml-system sfml-audio)
