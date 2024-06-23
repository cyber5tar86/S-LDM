cmake_minimum_required(VERSION 3.15)

project(
    s-ldm-dev
    VERSION 1.1.10
    LANGUAGES CXX
    DESCRIPTION "S-LDM"
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

add_subdirectory(components)