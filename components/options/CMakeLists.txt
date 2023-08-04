cmake_minimum_required(VERSION 3.15)

project(options
    VERSION 1.0.0
    LANGUAGES C
    DESCRIPTION "Options"
)

add_library(${PROJECT_NAME} OBJECT)

target_sources(${PROJECT_NAME} PRIVATE
    src/options_strings.c
    src/options.c
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

set_property(
    TARGET ${PROJECT_NAME}
    PROPERTY PUBLIC_HEADER
        include/options_strings.h
        include/options.h
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