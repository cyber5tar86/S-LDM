cmake_minimum_required(VERSION 3.15)

project(
    s-ldm
    VERSION 1.0.0
    LANGUAGES CXX
    DESCRIPTION "S-LDM"
)

find_package(AMQP REQUIRED)
find_package()

add_executable(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE
    include
)

target_sources(${PROJECT_NAME} PRIVATE
    src/amqpclient.h
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC calculator-shared-export::calculator-shared-export
)

    # copy the .dll file to the same folder as the executable
add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND
        ${CMAKE_COMMAND} -E copy_directory
        $<TARGET_FILE_DIR:calculator-shared-export::calculator-shared-export>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    VERBATIM
)

install(TARGETS s-ldm DESTINATION "."
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
)
