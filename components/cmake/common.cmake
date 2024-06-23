
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

# CheckCCompilerFlag serves to check if the compiler supports a given flag
include(CheckCCompilerFlag)

# Check if the compiler supports -fsplit-dwarf-inlining
check_c_compiler_flag("-fsplit-dwarf-inlining" HAVE_FSPLIT_DWARF_INLINING)

if(HAVE_FSPLIT_DWARF_INLINING)
    target_compile_options(${TARGET_NAME}
    PRIVATE
        "$<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>:-g>"
        "$<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>:-fsplit-dwarf-inlining>"
    )
endif()

set_target_properties(${TARGET_NAME}
PROPERTIES
    DEBUG_POSTFIX ".d"
)

# generates header to provide export macros for library
include(GenerateExportHeader)
generate_export_header(${TARGET_NAME} BASE_NAME ${TARGET_NAME})

install(
    TARGETS ${TARGET_NAME}
    EXPORT sldmTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sldm
    FILE_SET publicHeaders DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sldm
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sldm
    COMPONENT sldm_library
)

install(
    EXPORT sldmTargets
    NAMESPACE sldm::
    DESTINATION ${CMAKE_MODULES_INSTALL_DIR}/${TARGET_NAME}-${CMAKE_PROJECT_VERSION}
    COMPONENT sldm_library
)