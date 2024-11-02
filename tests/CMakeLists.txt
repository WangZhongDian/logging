project(test)

enable_testing()

#测试简单基本应用
add_executable(${PROJECT_NAME}simple test_simple.c)
target_link_libraries(${PROJECT_NAME}simple logging)
if(UNIX)
    add_test(test_simple ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}simple)
elseif(WIN32)
    add_test(test_simple ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}simple.exe)
endif()

#测试拦截器
add_executable(${PROJECT_NAME}interceptor test_interceptor.c)
target_link_libraries(${PROJECT_NAME}interceptor logging)
if(UNIX)
    add_test(test_interceptor ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}interceptor)
elseif(WIN32)
    add_test(test_interceptor ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}interceptor.exe)
endif()