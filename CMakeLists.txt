cmake_minimum_required( VERSION 3.28)

project(Logging)


aux_source_directory(${CMAKE_SOURCE_DIR}/src SRC)

set(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR}/include)

if (SHARED)
    add_library(${PROJECT_NAME} SHARED ${SRC})
else()
    add_library(${PROJECT_NAME} ${SRC})
endif()

if (SKIPTEST)
else()
    add_executable(test_simple ${CMAKE_SOURCE_DIR}/test_package/test_simple.c ${SRC})
    add_executable(test_interceptor ${CMAKE_SOURCE_DIR}/test_package/test_interceptor.c ${SRC})
endif()

