cmake_minimum_required(VERSION 2.8.11)

project(givr)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(GIVR_GLAD_INCLUDE_DIR "" CACHE FILEPATH "Path to glad includes")

set(DEFINITIONS -D_USE_MATH_DEFINES=1 GLM_FORCE_CXX14=1)

if(NOT GLM_FOUND)
    find_package(GLM REQUIRED)
endif()

find_file(GIVR_GLAD_H "glad.h" ${GIVR_GLAD_INCLUDE_DIR}/glad)
if(NOT GIVR_GLAD_H)
    message(SEND_ERROR "Can't find glad/glad.h in ${GIVR_GLAD_INCLUDE_DIR}")
endif()

include_directories(${GIVR_GLAD_INCLUDE_DIR} src)
file(GLOB SOURCES src/* src/givr/* src/givr/gl/* src/givr/styles/* src/givr/geometry/* src/givr/camera/*)

add_library(givr ${SOURCES})
target_compile_definitions(givr PRIVATE ${DEFINITIONS})
target_link_libraries(givr glm::glm)

