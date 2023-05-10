cmake_minimum_required(VERSION 3.25)
project(ex1 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(ex1
        IsraeliQueue.c
        IsraeliQueue.h main.c HackEnrollment.h HackEnrollment.c)
