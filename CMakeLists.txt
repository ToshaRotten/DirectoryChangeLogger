cmake_minimum_required(VERSION 3.24)
project(DirectoryChangeLogger)

set(CMAKE_CXX_STANDARD 23)

add_executable(DirectoryChangeLogger main.cpp loger/json_formatter/json_formatter.h loger/yaml_formatter/yaml_formatter.h loger/csv_formatter/csv_formatter.h )
