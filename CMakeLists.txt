cmake_minimum_required(VERSION 3.15)

project(
    s-ldm-dev
    VERSION 1.1.10
    LANGUAGES CXX
    DESCRIPTION "S-LDM"
)

add_subdirectory(components)
add_subdirectory(sldm)