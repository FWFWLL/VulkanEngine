cmake_minimum_required(VERSION 3.5)

SET(TARGET HelloCMake)

project(${TARGET})

file(GLOB_RECURSE SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

