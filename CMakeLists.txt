cmake_minimum_required( VERSION 3.28)
project(logging)

option(TEST "是否启动单元测试" ON)
option(SHARED "是否编译为动态库" OFF)

set(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR}/include)

#编译库文件
add_subdirectory(src)

#测试单元
if (TEST)
    enable_testing()
    add_subdirectory(tests)
endif()
