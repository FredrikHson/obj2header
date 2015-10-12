cmake_minimum_required(VERSION 2.8)

project(obj2header)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(COMMON_SRC
    "source/main.cpp"
    "fast_obj_loader/fast_obj_loader.cpp"
    )

include_directories(
    "source/"
    "fast_obj_loader/"
    )

add_executable(${CMAKE_PROJECT_NAME} ${COMMON_SRC})

target_link_libraries(${CMAKE_PROJECT_NAME}
    rt
    )
