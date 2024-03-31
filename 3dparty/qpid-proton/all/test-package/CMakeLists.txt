cmake_minimum_required(VERSION 3.15)
project(QpidProtonTestPackage CXX)

find_package(qpid-proton REQUIRED)

add_executable(example)

target_sources(example PRIVATE main.cpp)
target_link_libraries(example PRIVATE qpid-proton::qpid-proton)
target_compile_features(example PRIVATE cxx_std_17)
