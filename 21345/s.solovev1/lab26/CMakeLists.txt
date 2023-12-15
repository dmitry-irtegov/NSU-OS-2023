cmake_minimum_required(VERSION 2.8)

project(lab26)

set(MAIN_EXE main.c)

set(PRINT_UPPER_EXE printInUpper.c)

add_executable(main ${MAIN_EXE})

add_executable(printInUpper ${PRINT_UPPER_EXE})
