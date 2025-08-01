cmake_minimum_required(VERSION 3.10)
project(system-software-lab C)

set(CMAKE_C_STANDARD 99)

# Automatically add all .c files in the current directory
file(GLOB SOURCES "*.c")

# Create a separate executable for each .c file
foreach(source_file ${SOURCES})
    get_filename_component(exec_name ${source_file} NAME_WE)
    add_executable(${exec_name} ${source_file})
endforeach()
