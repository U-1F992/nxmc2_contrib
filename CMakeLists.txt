cmake_minimum_required(VERSION 3.10)

project(nxmc2_contrib)
set(CMAKE_C_STANDARD 99)

include_directories(include)
add_library(nxmc2_contrib STATIC 
            src/nxmc2_command.c
            src/nxmc2_command_builder.c
            src/nxmc2_command_handler.c)

# Create test executable
add_executable(test test/test.c)
target_link_libraries(test nxmc2_contrib)