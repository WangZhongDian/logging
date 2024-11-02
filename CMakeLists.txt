cmake_minimum_required(VERSION 3.28...3.30)

set(CMAKE_EXPORT_COMPILE_COMMANDS yes)

project(logging)

option(TEST "是否启动单元测试" ON)
option(SHARED "是否编译为动态库" OFF)

include_directories(${CMAKE_SOURCE_DIR}/include)

#编译库文件
add_subdirectory(src)

#测试单元
if (TEST)
    enable_testing()
    add_subdirectory(tests)
endif()
