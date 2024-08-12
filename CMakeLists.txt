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


add_executable(test_main ${CMAKE_SOURCE_DIR}/test_package/main.c ${SRC})


