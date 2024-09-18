project(test)

add_executable(${PROJECT_NAME}simple test_simple.c)
target_link_libraries(${PROJECT_NAME}simple Logging)


add_executable(${PROJECT_NAME}interceptor test_interceptor.c)
target_link_libraries(${PROJECT_NAME}interceptor Logging)
