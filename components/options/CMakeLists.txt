cmake_minimum_required(VERSION 3.27)

set(COMPONENT_NAME options)
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
            include/options_strings.h
            include/options.h
    PRIVATE
            src/options_strings.c
            src/options.c
)

target_include_directories(${TARGET_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

include(common)