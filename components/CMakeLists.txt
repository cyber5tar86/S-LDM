cmake_minimum_required(VERSION 3.15)

project(s-ldm-components
    LANGUAGES C CXX
)

#add_subdirectory(asn1)
add_subdirectory(decoder-module)
add_subdirectory(options)
add_subdirectory(vehicle-visualizer)

install(
    EXPORT s-ldm-config
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

configure_file(${PROJECT_NAME}-config.cmake.in ${PROJECT_NAME}-config.cmake @ONLY)

install(
    FILES "${CMAKE_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)