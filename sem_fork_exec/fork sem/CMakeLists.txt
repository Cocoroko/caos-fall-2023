cmake_minimum_required(VERSION 3.24)
project(fork_sem C)

set(CMAKE_C_STANDARD 11)

add_executable(fork_sem fork_bomb.c)
