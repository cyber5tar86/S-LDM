cmake_minimum_required(VERSION 3.15)

project(s-ldm
    VERSION 1.0.0
    LANGUAGES C CXX
)

# add the folder 'cmake' to the available modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(sldmDependencies.cmake)

add_subdirectory(logger)
add_subdirectory(asn1)
add_subdirectory(decoder-module)
add_subdirectory(options)
add_subdirectory(vehicle-visualizer)
add_subdirectory(sldm)

# project-wide wrapper
add_library(sldm INTERFACE)
add_library(sldm::sldm ALIAS sldm)

target_link_libraries(sldm
    INTERFACE
        sldm::asn1
        sldm::decoder-module
        sldm::logger
        sldm::options
        sldm::sldm
        sldm::vehicle-visualizer
)

install(
    TARGETS sldm
    EXPORT sldmTargets
    COMPONENT sldm_library
)

# local build
export(EXPORT sldmTargets NAMESPACE sldm::)
configure_file("sldmConfig.cmake" "." COPYONLY)
include(CMakePackageConfigHelpers)
write_basic_package_version_file(sldmConfigVersion.cmake COMPATIBILITY SameMajorVersion)

# installation
install(
    EXPORT sldmTargets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/sldm
    NAMESPACE sldm::
    COMPONENT sldm_library
)

install(
    FILES sldmConfig.cmake sldmDependencies.cmake ${CMAKE_CURRENT_BINARY_DIR}/sldmConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/sldm
    COMPONENT sldm_library
)