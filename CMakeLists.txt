cmake_minimum_required(VERSION 3.5)
project(MARIECPP)

file(GLOB_RECURSE SRC_FILES "src/*")
add_executable(MARIECPP ${SRC_FILES})