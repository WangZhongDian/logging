project(test)

enable_testing()

#测试简单基本应用
add_executable(${PROJECT_NAME}simple test_simple.c)
target_link_libraries(${PROJECT_NAME}simple logging)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_test(test_simple ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME}simple)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_test(test_simple ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME}simple.exe)
endif()

#测试拦截器
add_executable(${PROJECT_NAME}interceptor test_interceptor.c)
target_link_libraries(${PROJECT_NAME}interceptor logging)
add_test(test_interceptor ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME}interceptor)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_test(test_interceptor ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME}interceptor)   
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_test(test_interceptor ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME}interceptor.exe)
endif()