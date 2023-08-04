cmake_minimum_required(VERSION 3.15)

project(logger
    VERSION 1.0.0
    LANGUAGES CXX
    DESCRIPTION "logger wrapper"
)

find_package(log4cplus REQUIRED CONFIG)

add_library(${PROJECT_NAME} OBJECT)

target_sources(${PROJECT_NAME} PRIVATE
    src/log.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_link_libraries(${PROJECT_NAME} PUBLIC
    log4cplus::log4cplus
)

set_property(
    TARGET ${PROJECT_NAME}
    PROPERTY PUBLIC_HEADER
        include/log.h
)

install(
    TARGETS ${PROJECT_NAME}
    EXPORT components-config
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/components/${PROJECT_NAME}
)

add_library(s-ldm::${PROJECT_NAME} ALIAS ${PROJECT_NAME})