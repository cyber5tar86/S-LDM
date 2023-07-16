cmake_minimum_required(VERSION 3.15)

project(s-ldm
    VERSION 1.1.10
    LANGUAGES CXX
    DESCRIPTION "S-LDM"
)

#find_package(qpid-proton-cpp REQUIRED)
find_package(asio REQUIRED)
find_package(Boost REQUIRED COMPONENTS)
find_package(cpprestsdk REQUIRED)
find_package(GeographicLib REQUIRED)
find_package(nlohmann_json REQUIRED CONFIG)
find_package(s-ldm-components REQUIRED CONFIG COMPONENTS asn1 decoder-module options vehicle-visualizer)

add_executable(s-ldm)

target_sources(s-ldm PRIVATE
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

target_include_directories(s-ldm PRIVATE
    include
)

target_link_libraries(s-ldm PRIVATE
    boost::boost
    crypto
    cpprestsdk::cpprest
    m
    nlohmann_json::nlohmann_json
    pthread
    qpid-proton-cpp
    GeographicLib::GeographicLib
)

target_compile_features(s-ldm PRIVATE cxx_std_17)

install(TARGETS s-ldm DESTINATION "."
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
)