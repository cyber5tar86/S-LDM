cmake_minimum_required(VERSION 3.15)

project(s-ldm
    VERSION 1.1.10
    LANGUAGES CXX
    DESCRIPTION "S-LDM"
)

#find_package(qpid-proton-cpp REQUIRED)
#find_package(asio REQUIRED)
find_package(Boost REQUIRED COMPONENTS)
find_package(cpprestsdk REQUIRED)
find_package(GeographicLib REQUIRED)
find_package(nlohmann_json REQUIRED CONFIG)
find_package(log4cplus REQUIRED)
#find_package(s-ldm-components REQUIRED CONFIG COMPONENTS asn1 decoder-module options vehicle-visualizer)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
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

target_include_directories(${PROJECT_NAME} PRIVATE
    include
    ../components/asn1/include
    ../components/options/include
    ../components/decoder-module/include
    ../components/vehicle-visualizer/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    boost::boost
    crypto
    cpprestsdk::cpprest
    s-ldm::logger
    m
    nlohmann_json::nlohmann_json
    pthread
    qpid-proton-cpp
    GeographicLib::GeographicLib
    decoder-module
    options
    vehicle-visualizer
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

install(
    TARGETS ${PROJECT_NAME} DESTINATION "."
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)