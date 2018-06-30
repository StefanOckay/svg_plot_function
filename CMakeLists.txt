cmake_minimum_required(VERSION 2.8)

project(hw06)
add_executable(${PROJECT_NAME} "main.c" "svggraph.c")
target_link_libraries(hw06 m)
