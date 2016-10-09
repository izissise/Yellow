cmake_minimum_required(VERSION 3.0.2)

project(Yellow)

set(BIN_NAME "yellow")
set(LIB_NAME "wire")
set(BIN_TEST "tester")

set(LIBINCLUDE_DIR "${PROJECT_SOURCE_DIR}/libwire/inc")
set(LIBSOURCE_DIR "${PROJECT_SOURCE_DIR}/libwire/src")
set(BININCLUDE_DIR "${PROJECT_SOURCE_DIR}/yellow/inc")
set(BINSOURCE_DIR "${PROJECT_SOURCE_DIR}/yellow/src")
set(TEST_DIR "${PROJECT_SOURCE_DIR}/tests")
set(DOC_PATH "${PROJECT_SOURCE_DIR}/doc")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

# cmake modules
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
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
            FLAGS_RELEASE  "-march=native" "-Wno-inline"
)

# --- user options ---
OPTION(BUILD_TESTS "Decide if the test suite shall be built or not." OFF)
CMAKE_DEPENDENT_OPTION(BUILD_TESTS_COVERAGE "Decide if a coverage report should be generated." ON "BUILD_TESTS" OFF)
CMAKE_DEPENDENT_OPTION(VALGRIND_CHECK "Decide if valgrind should be executed with the test binary to check for memory leaks." ON "BUILD_TESTS" OFF)
OPTION(CODE_METRICS "Decide if code's metrics should be generated." OFF)
OPTION(CODE_CHECK   "Decide if code should be checked for common errors." OFF)
OPTION(GENERATE_DOC "Decide if documentation should be generated." OFF)

include_directories("${LIBINCLUDE_DIR}")

# NetworkLib
# Build it here in ${CMAKE_CURRENT_BINARY_DIR}/network
# set(NetworkLib_DIR "${CMAKE_CURRENT_BINARY_DIR}/network")
# file(MAKE_DIRECTORY ${NetworkLib_DIR})
# execute_process(COMMAND ${CMAKE_COMMAND} "${PROJECT_SOURCE_DIR}/network" "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}" "-G" "${CMAKE_GENERATOR}" WORKING_DIRECTORY "${NetworkLib_DIR}")
# execute_process(COMMAND ${CMAKE_COMMAND} "--build" "${NetworkLib_DIR}" WORKING_DIRECTORY "${NetworkLib_DIR}")
# find_package(NetworkLib REQUIRED)

# All source files using globs
file(
  GLOB
  lib_source_files
  ${LIBSOURCE_DIR}/*
)

file(
  GLOB_RECURSE
  lib_include_files
  ${LIBINCLUDE_DIR}/*
)

file(
  GLOB
  bin_source_files
  ${BINSOURCE_DIR}/*
)

file(
  GLOB_RECURSE
  bin_include_files
  ${BININCLUDE_DIR}/*
)

add_library(
# executable name
  ${LIB_NAME} STATIC
# source files
  ${lib_source_files}
  ${lib_include_files}
)

add_executable(
# executable name
  ${BIN_NAME}
# source files
  ${bin_source_files}
  ${bin_include_files}
)

# set_property(TARGET ${BIN_NAME} APPEND PROPERTY INCLUDE_DIRECTORIES "${LIBINCLUDE_DIR}")
# set_property(TARGET ${LIB_NAME} APPEND PROPERTY INCLUDE_DIRECTORIES "${LIBINCLUDE_DIR}")
set_property(TARGET ${BIN_NAME} APPEND PROPERTY INCLUDE_DIRECTORIES "${BININCLUDE_DIR}")
target_link_libraries(${BIN_NAME} ${LIB_NAME})

# include_directories("${NETWORKLIB_INCLUDE_DIRS}")
# set_property(TARGET ${LIB_NAME} APPEND PROPERTY INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/${LIB_NAME}/inc")

# target_link_libraries(${LIB_NAME} NETWORKLIB_network)
# target_link_libraries(${LIB_NAME} ${CMAKE_THREAD_LIBS_INIT})

file(GLOB TEST_FILES "${TEST_DIR}/*.cpp")

# if (THREADS_FOUND)
#   target_link_libraries (${LIB_NAME} ${CMAKE_THREAD_LIBS_INIT})
# endif()

# Add test target
# --- enable unit tests if desired ---
if(BUILD_TESTS)
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
