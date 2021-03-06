cmake_minimum_required(VERSION 3.0.2)

set(PROJECT_NAME "Yellow")
project(${PROJECT_NAME})

set(BIN_NAME "yellow")
set(LIB_NAME "wire")
set(BIN_TEST "tester")

set(LIBINCLUDE_DIR "${PROJECT_SOURCE_DIR}/libwire/inc")
set(LIBSOURCE_DIR "${PROJECT_SOURCE_DIR}/libwire/src")
set(BINSOURCE_DIR "${PROJECT_SOURCE_DIR}/yellow/src")
set(BININCLUDE_DIR "${PROJECT_SOURCE_DIR}/yellow/inc")
set(BINRESSOURCESOURCE_DIR "${PROJECT_SOURCE_DIR}/yellow/res")
set(TEST_DIR "${PROJECT_SOURCE_DIR}/tests")
set(DOC_PATH "${PROJECT_SOURCE_DIR}/doc")


set(BINRESSOURCE_DIR "${BINRESSOURCESOURCE_DIR}")

# meta
# ******************************************************************************
set(TEMPLATE_VERSION_MAJOR 1 CACHE INT "Version Major")
set(TEMPLATE_VERSION_MINOR 0 CACHE INT "Version Minor")
set(TEMPLATE_VERSION_PATCH 0 CACHE INT "Version Patch")
set(TEMPLATE_ORGANIZATION "NSA" CACHE STRING "Organization")
set(TEMPLATE_DOMAIN "Listening" CACHE STRING "Domain")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

# cmake modules
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake;")
# find_package(Threads REQUIRED)

INCLUDE(CMakeDependentOption)
INCLUDE(CheckIncludeFile)
INCLUDE(EnableSlocCount)
include(EnableTests)
INCLUDE(GenerateDoxygen)
INCLUDE(GenerateCppcheck)
INCLUDE(GenerateCccc)
INCLUDE(GenerateValgrind)
INCLUDE(CheckCompilerFlags)
INCLUDE(GenerateExportHeader)
INCLUDE(Functions)

# --- global compiler flags ---
COMPILER_FLAGS(FLAGS "-std=c++14"
                    "-Wall" "-pedantic" "-Wextra"
                    "-pedantic-errors" "-Wcast-align"
                    "-Wcast-qual" "-Wconversion"
                    "-Wdisabled-optimization"
                    "-Wdocumentation"
                    "-Wformat=2"
                    "-Wformat-nonliteral" "-Wformat-security"
                    "-Wformat-y2k"
                    "-Wimplicit" "-Wimport" "-Winit-self" "-Winline"
                    "-Wlong-long"
                    "-Wmissing-field-initializers" "-Wmissing-format-attribute"
                    "-Wmissing-include-dirs" "-Wmissing-noreturn"
                    "-Wpacked" "-Wpointer-arith"
                    "-Wredundant-decls"
                    "-Wstack-protector"
                    "-Wstrict-aliasing=2" "-Wswitch-default"
                    "-Wswitch-enum"
                    "-Wunreachable-code" "-Wunused"
                    "-Wunused-parameter"
                    "-Wvariadic-macros"
                    "-Wno-builtin-macro-redefined"
                    "-Wwrite-strings"
                    "-Wno-unknown-pragmas"
                    "-Wno-suggest-attribute=noreturn"
                    "-fsanitize=address"
                    "-pipe"
                    "-pthread"
            FLAGS_RELEASE  "-march=native" "-Wno-inline"
)

# --- user options ---
OPTION(BUILD_APP "Decide if the app shall be built or not." ON)
OPTION(BUILD_TESTS "Decide if the test suite shall be built or not." OFF)
OPTION(INSTALLABLE "Make build installable, neccessary for package target." OFF)
CMAKE_DEPENDENT_OPTION(BUILD_TESTS_COVERAGE "Decide if a coverage report should be generated." ON "BUILD_TESTS" OFF)
CMAKE_DEPENDENT_OPTION(VALGRIND_CHECK "Decide if valgrind should be executed with the test binary to check for memory leaks." ON "BUILD_TESTS" OFF)
OPTION(CODE_METRICS "Decide if code's metrics should be generated." OFF)
OPTION(CODE_CHECK   "Decide if code should be checked for common errors." OFF)
OPTION(GENERATE_DOC "Decide if documentation should be generated." OFF)


if(INSTALLABLE)
    set(BINRESSOURCE_DIR "/usr/share/yellow/res")
endif()

# sources
# ******************************************************************************
AddSources("${LIBSOURCE_DIR}" "Lib" lib_source_files)
AddSources("${LIBINCLUDE_DIR}" "Lib" lib_include_files)

AddSources("${BINSOURCE_DIR}" "App" bin_source_files)
AddSources("${BININCLUDE_DIR}" "App" bin_include_files)

# configure files
# ******************************************************************************
configure_file(${BININCLUDE_DIR}/meta.h.in meta.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(
# executable name
  ${LIB_NAME} STATIC
# source files
  ${lib_source_files}
  ${lib_include_files}
)

# Only if we build the app
if (BUILD_APP)
    add_executable(
        # executable name
        ${BIN_NAME}
        # source files
        ${bin_source_files}
        ${bin_include_files}
    )


    # Lib link
    # ********************
    include_directories("${LIBINCLUDE_DIR}")
    # set_property(TARGET ${BIN_NAME} APPEND PROPERTY INCLUDE_DIRECTORIES "${LIBINCLUDE_DIR}")
    # set_property(TARGET ${LIB_NAME} APPEND PROPERTY INCLUDE_DIRECTORIES "${LIBINCLUDE_DIR}")
    set_property(TARGET ${BIN_NAME} APPEND PROPERTY INCLUDE_DIRECTORIES "${BININCLUDE_DIR}")
    target_link_libraries(${BIN_NAME} PUBLIC ${LIB_NAME})


    ## QT5 ##
    set_target_properties(${BIN_NAME}
        PROPERTIES
        AUTOMOC ON
        AUTOUIC ON
    )
    target_link_libraries(${BIN_NAME}
        PUBLIC
        Qt5::Core
        Qt5::Widgets
        Qt5::Gui
        Qt5::Qml
        Qt5::Quick
        Qt5::Concurrent
    )
    find_package(Qt5 COMPONENTS Core Gui Widgets Concurrent Qml Quick REQUIRED)

    set_target_properties(${BIN_NAME} PROPERTIES AUTOMOC ON)
    set_target_properties(${BIN_NAME} PROPERTIES AUTOUIC ON)
    target_link_libraries(${BIN_NAME} PUBLIC Qt5::Core PUBLIC Qt5::Widgets PUBLIC Qt5::Gui PUBLIC Qt5::Qml PUBLIC Qt5::Quick PUBLIC Qt5::Concurrent)

    #########
endif()

# Tests and Metrics
# *****************************
AddSources("${TEST_DIR}" "Test" TEST_FILES)

# if (THREADS_FOUND)
#   target_link_libraries (${LIB_NAME} ${CMAKE_THREAD_LIBS_INIT})
# endif()

# Add test target
# --- enable unit tests if desired ---
if(BUILD_TESTS)
  AddSources("${TEST_DIR}/helpers" "Test" test_helper_files)
  AddSources("${TEST_DIR}/testFiles" "Test" test_tfiles_files)

  set(TEST_FILES ${TEST_FILES} ${test_helper_files} ${test_tfiles_files})
  GENERATE_TEST_TARGET(COVERAGE ${BUILD_TESTS_COVERAGE} VALGRIND ${VALGRIND_CHECK}
                       TARGET_NAME ${BIN_TEST} FILES ${TEST_FILES} LIB_TARGET ${LIB_NAME})
endif()


#Extra targets:
if(CODE_CHECK)
  INCLUDE(GenerateCppcheck)
  # --- cppcheck ---
  GENERATE_CPPCHECK(SOURCES "${LIBSOURCE_DIR}" "${BINSOURCE_DIR}"
                            "${LIBINCLUDE_DIR}" "${BININCLUDE_DIR}"
                            "${TEST_DIR}"
                            ENABLE_IDS style
                            INLINE_SUPPRESSION)
endif()

if(CODE_METRICS)
  INCLUDE(GenerateCccc)
  # --- cccc ---
  GENERATE_CCCC(SOURCES "${PROJECT_SOURCE_DIR}/${SOURCE_DIR}"
                        "${PROJECT_SOURCE_DIR}/${INCLUDE_DIR}"
                        "${PROJECT_SOURCE_DIR}/${TEST_DIR}")
  INCLUDE(EnableSlocCount)
  # --- SlocCount --- #
  ENABLE_SLOCCOUNT(FOLDERS "${PROJECT_SOURCE_DIR}/${SOURCE_DIR}"
                            "${PROJECT_SOURCE_DIR}/${INCLUDE_DIR}"
                            "${PROJECT_SOURCE_DIR}/${TEST_DIR}")
endif()

# Generate Doxygen Doc
if(GENERATE_DOC)
  INCLUDE(GenerateDoxygen)
  # --- Doxygen --- #
  GENERATE_DOXYGEN(SOURCES "${PROJECT_SOURCE_DIR}/${SOURCE_DIR}"
                    "${PROJECT_SOURCE_DIR}/${INCLUDE_DIR}"
                    "${PROJECT_SOURCE_DIR}/${TEST_DIR}" ${DOC_PATH}
        VERSION "${${PROJECT_NAME}_VERSION}")
endif()


if (BUILD_APP)
    # Deb package generation
    INSTALL(TARGETS ${BIN_NAME}
            RUNTIME DESTINATION "bin"
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib/static)
    INSTALL(DIRECTORY ${BINRESSOURCESOURCE_DIR} DESTINATION "/usr/share/yellow/")


    SET(CPACK_GENERATOR "DEB")
    SET(CPACK_PACKAGE_MAINTAINER "Yellow's project menbers")
    SET(CPACK_PACKAGE_CONTACT "noreply@example.com")
    SET(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
    SET(CPACK_PACKAGE_VERSION_MAJOR "${TEMPLATE_VERSION_MAJOR}")
    SET(CPACK_PACKAGE_VERSION_MINOR "${TEMPLATE_VERSION_MINOR}")
    SET(CPACK_PACKAGE_VERSION_PATCH "${TEMPLATE_VERSION_PATCH}")
    SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Sniff Sniff")
    SET(CPACK_DEBIAN_PACKAGE_SECTION "stable")
    SET(CPACK_PACKAGE_DEBIAN_ARCH "amd64")
    SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt5gui5 (>= 5.6)")

    INCLUDE(CPack)
endif()

