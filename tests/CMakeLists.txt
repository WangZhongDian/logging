project(test)

enable_testing()

#测试简单基本应用
add_executable(${PROJECT_NAME}simple test_simple.c)
target_link_libraries(${PROJECT_NAME}simple Logging)
add_test(test_simple ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME}simple)


#测试拦截器
add_executable(${PROJECT_NAME}interceptor test_interceptor.c)
target_link_libraries(${PROJECT_NAME}interceptor Logging)
add_test(test_interceptor ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME}interceptor)