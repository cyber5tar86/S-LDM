cmake_minimum_required(VERSION 3.27)

set(COMPONENT_NAME decoder-module)
set(TARGET_NAME ${PROJECT_NAME}_${COMPONENT_NAME})

add_library(${TARGET_NAME} STATIC)
add_library(${PROJECT_NAME}::${COMPONENT_NAME} ALIAS ${TARGET_NAME})

set_target_properties(${TARGET_NAME}
    PROPERTIES
        VERSION ${PROJECT_VERSION}
        OUTPUT_NAME ${COMPONENT_NAME}
        EXPORT_NAME ${COMPONENT_NAME}
        POSITION_INDEPENDENT_CODE ON
)

target_sources(${TARGET_NAME}
    PUBLIC
        FILE_SET publicHeaders
        TYPE HEADERS
        FILES
            include/basicheader.h
            include/btp.h
            include/btpheader.h
            include/commonheader.h
            include/etsidecoderfrontend.h
            include/gbcheader.h
            include/geonet.h
            include/shbheader.h
    PRIVATE
            src/basicheader.cpp
            src/btp.cpp
            src/btpheader.cpp
            src/commonheader.cpp
            src/etsidecoderfrontend.cpp
            src/gbcheader.cpp
            src/geonet.cpp
            src/shbheader.cpp
)

target_include_directories(${TARGET_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(${TARGET_NAME}
    PUBLIC
        s-ldm::asn1
        s-ldm::logger
)

include(common)