cmake_minimum_required(VERSION 2.8)

project(obj2header)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(COMMON_SRC
    "source/main.cpp"
    )

include_directories(
    "source/"
    )

add_executable(${CMAKE_PROJECT_NAME} ${COMMON_SRC})

target_link_libraries(${CMAKE_PROJECT_NAME}
    )
