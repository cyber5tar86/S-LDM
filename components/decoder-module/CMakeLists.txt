cmake_minimum_required(VERSION 3.15)

project(decoder-module
    VERSION 1.0.0
    LANGUAGES CXX
    DESCRIPTION "Decoder Module"
)

add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
    src/basicheader.cpp
    src/btp.cpp
    src/btpheader.cpp
    src/commonheader.cpp
    src/etsidecoderfrontend.cpp
    src/gbcheader.cpp
    src/geonet.cpp
    src/shbheader.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    asn1/include
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    asn1
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

set_property(
    TARGET ${PROJECT_NAME}
    PROPERTY PUBLIC_HEADER
        include/basicheader.h
        include/btp.h
        include/btpheader.h
        include/commonheader.h
        include/etsidecoderfrontend.h
        include/gbcheader.h
        include/geonet.h
        include/shbheader.h
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