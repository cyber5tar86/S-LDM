cmake_minimum_required(VERSION 3.15)

project(vehicle-visualizer
    VERSION 1.0.0
    LANGUAGES CXX
    DESCRIPTION "Vehicle Visualizer"
)

add_library(${PROJECT_NAME} OBJECT)

target_sources(${PROJECT_NAME} PRIVATE
    src/vehicle-visualizer.cc
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    s-ldm::logger
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

set_property(
    TARGET ${PROJECT_NAME}
    PROPERTY PUBLIC_HEADER
        include/vehicle-visualizer.h
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