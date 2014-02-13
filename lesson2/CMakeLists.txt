project(getimage)
cmake_minimum_required(VERSION 2.8)

file(GLOB ALL_SRC_LIST
    "*.h"
    "*.cpp"
)

add_executable(${PROJECT_NAME} ${ALL_SRC_LIST})

