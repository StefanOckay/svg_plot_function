cmake_minimum_required(VERSION 2.8)

project(svg_plot_function)
add_executable(${PROJECT_NAME} "main.c" "svggraph_table.c" "svggraph_write.c" "svggraph_input.c")

target_link_libraries(svg_plot_function m)
