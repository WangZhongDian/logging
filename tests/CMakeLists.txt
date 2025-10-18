project(test)

enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/src)

#测试简单基本应用
add_executable(${PROJECT_NAME}simple test-simple.c)
target_link_libraries(${PROJECT_NAME}simple logging)
add_test(test_simple ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}simple${CMAKE_EXECUTEABLE_SUFFIX})

#测试简单基本应用
add_executable(${PROJECT_NAME}file test-log-file.c)
target_link_libraries(${PROJECT_NAME}file logging)
add_test(test_file ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}file${CMAKE_EXECUTEABLE_SUFFIX})


#测试拦截器
add_executable(${PROJECT_NAME}filter test-filter.c)
target_link_libraries(${PROJECT_NAME}filter logging)
add_test(test_filter ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}filter${CMAKE_EXECUTEABLE_SUFFIX})

#测试多log多次获取
add_executable(${PROJECT_NAME}logs test-logs.c)
target_link_libraries(${PROJECT_NAME}logs logging)
add_test(test_logs ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}logs${CMAKE_EXECUTEABLE_SUFFIX})

#测试工具map
add_executable(${PROJECT_NAME}map test-map.c)
target_link_libraries(${PROJECT_NAME}map logging)
add_test(test_map ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}map${CMAKE_EXECUTEABLE_SUFFIX})