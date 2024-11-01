project(logging)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/handler SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/interceptor SRC)

if (SHARED)
    add_library(${PROJECT_NAME} SHARED ${SRC})
else()
    add_library(${PROJECT_NAME} ${SRC})
endif()

