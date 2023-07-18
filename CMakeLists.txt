cmake_minimum_required(VERSION 3.15)

project(
    s-ldm-dev
    VERSION 1.1.10
    LANGUAGES CXX
    DESCRIPTION "S-LDM"
)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
add_subdirectory(components)
add_subdirectory(sldm)