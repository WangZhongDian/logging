cmake_minimum_required(VERSION 3.28...3.30)

project(logging)

set(CMAKE_C_STANDARD 99)

if(MSVC)
    add_compile_options("/source-charset:utf-8")
    add_compile_options("/execution-charset:utf-8")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS yes)

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


install(FILES include/logging.h DESTINATION include)
install(FILES include/logging/logging-core.h DESTINATION include/logging)
install(FILES include/logging/logging-filter.h DESTINATION include/logging)
install(FILES include/logging/logging-handler.h DESTINATION include/logging)
