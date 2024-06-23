cmake_minimum_required(VERSION 3.27)

set(COMPONENT_NAME sldm)
set(TARGET_NAME ${PROJECT_NAME}_${COMPONENT_NAME})

add_executable(${TARGET_NAME})
add_executable(${PROJECT_NAME}::${COMPONENT_NAME} ALIAS ${TARGET_NAME})

set_target_properties(${TARGET_NAME}
    PROPERTIES
        OUTPUT_NAME ${COMPONENT_NAME}
        EXPORT_NAME ${COMPONENT_NAME}
)

target_sources(${TARGET_NAME}
    PUBLIC
        FILE_SET publicHeaders
        TYPE HEADERS
        FILES
            include/ampqclientconfiguration.h
            include/amqpclient.h
            include/areafilter.h
            include/configmanager.h
            include/jsonserver.h
            include/ldmmap.h
            include/maneuveringservicerestclient.h
            include/phpoints.h
            include/quadkeyts.h
            include/timers.h
            include/triggermanager.h
            include/utils.h
            include/vehicledatadef.h
    PRIVATE
            src/amqpclient.cpp
            src/areafilter.cpp
            src/jsonserver.cpp
            src/ldmmap.cpp
            src/main.cpp
            src/maneuveringservicerestclient.cpp
            src/phpoints.cpp
            src/quadkeyts.cpp
            src/timers.cpp
            src/triggermanager.cpp
            src/utils.cpp
)

target_include_directories(${TARGET_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(${TARGET_NAME}
    PRIVATE
        boost::boost
        crypto
        cpprestsdk::cpprest
        s-ldm::logger
        m
        nlohmann_json::nlohmann_json
        pthread
        qpid-proton-cpp
        GeographicLib::GeographicLib
        s-ldm::decoder-module
        s-ldm::options
        s-ldm::vehicle-visualizer
)

# CheckCCompilerFlag serves to check if the compiler supports a given flag
include(CheckCCompilerFlag)

# Check if the compiler supports -fsplit-dwarf
check_c_compiler_flag("-fsplit-dwarf" HAVE_FSPLIT_DWARF)

if(HAVE_FSPLIT_DWARF)
    target_compile_options(${TARGET_NAME}
        PRIVATE
            "$<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>:-g>"
            "$<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>:-fsplit-dwarf>"
    )

    set_target_properties(${TARGET_NAME}
        PROPERTIES
            DEBUG_POSTFIX ".d"
    )
endif()

# Install the binary
install(
    TARGETS ${TARGET_NAME}
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}
)