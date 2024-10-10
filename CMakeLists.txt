cmake_minimum_required( VERSION 3.28)
project(Logging)

enable_testing()

set(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR}/include)

#编译库文件
add_subdirectory(src)

#测试单元
if (SKIPTEST)
else()
    add_subdirectory(tests)
endif()
