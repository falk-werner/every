cmake_minimum_required (VERSION 3.20)
project(every VERSION 1.0.0 DESCRIPTION "Repeat command")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(every src/main.c)
target_compile_options(every PRIVATE -Werror -Wall -Wextra -Wpedantic)

install(TARGETS every RUNTIME DESTINATION bin)
