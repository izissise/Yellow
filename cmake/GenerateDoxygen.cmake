# - Generate a doxygen documentation for a project.
# The function GENERATE_DOXYGEN is provided to create a "doc" target that
# generates a doxygen documentation (currently only as HTML report).
#
# GENERATE_DOXYGEN([AUTO_INSTALL] [NAME <project name>] [VERSION <project version>] [INSTALL_DIR <doc install dir>])
#
#     AUTO_INSTALL: if given the documentation is automatically generated with
#                   the install target and then installed (sometimes buggy)
#     NAME: name of the project used for the documentation, defaults to
#           CMAKE_PROJECT_NAME
#     VERSION: version of the software, if not given, no version is passed to
#              doxygen
#     INSTALL_DIR: directory under which to install the generated documentation.
#                  If not given, this defaults to
#                  ${CMAKE_INSTALL_PREFIX}/share/${NAME}
#
# This function can always be called, even if no doxygen was found. Then no
# target is created.
#
# Copyright (C) 2010 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
#
# This file may be licensed under the terms of the
# GNU Lesser General Public License Version 3 (the ``LGPL''),
# or (at your option) any later version.
#
# Software distributed under the License is distributed
# on an ``AS IS'' basis, WITHOUT WARRANTY OF ANY KIND, either
# express or implied. See the LGPL for the specific language
# governing rights and limitations.
#
# You should have received a copy of the LGPL along with this
# program. If not, go to http://www.gnu.org/licenses/lgpl.html
# or write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#
# The development of this software was supported by:
#   CoR-Lab, Research Institute for Cognition and Robotics
#     Bielefeld University

GET_FILENAME_COMPONENT(GENERATE_DOXYGEN_MODULE_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)

INCLUDE(ParseArguments)
FIND_PACKAGE(Doxygen)

FUNCTION(GENERATE_DOXYGEN)

  IF(DOXYGEN_FOUND)
    # argument parsing
    PARSE_ARGUMENTS(ARG "SOURCES;NAME;VERSION;INSTALL_DIR" "AUTO_INSTALL" ${ARGN})

    # name
    STRING(TOLOWER "${CMAKE_PROJECT_NAME}" DOC_PROJECT_NAME)
    LIST(LENGTH ARG_NAME NAME_LENGTH)
    IF(${NAME_LENGTH} EQUAL 1)
        SET(DOC_PROJECT_NAME "${ARG_NAME}")
    ENDIF()

    # version
    SET(DOC_PROJECT_VERSION "")
    LIST(LENGTH ARG_VERSION VERSION_LENGTH)
    IF(${VERSION_LENGTH} EQUAL 1)
        SET(DOC_PROJECT_VERSION "${ARG_VERSION}")
    ENDIF()

    # write a list file containing all sources to check for the call to
    # doxygen
    SET(DOC_SOURCES "")
    FOREACH(SOURCE ${ARG_SOURCES})
        SET(DOC_SOURCES "${DOC_SOURCES} \"${SOURCE}\"")
    ENDFOREACH()

    # install dir
    SET(DOC_INSTALL_DIR "share/${DOC_PROJECT_NAME}")
    LIST(LENGTH ARG_INSTALL_DIR INSTALL_DIR_LENGTH)
    IF(${INSTALL_DIR_LENGTH} EQUAL 1)
        SET(DOC_INSTALL_DIR "${ARG_INSTALL_DIR}")
    ENDIF()

    SET(DOC_API_DIR ${CMAKE_BINARY_DIR}/doc)
    SET(DOC_BASE_SOURCE_DIR ${CMAKE_SOURCE_DIR})
    SET(DOC_WARN_FILE ${CMAKE_BINARY_DIR}/doxygen-warn.log)

    SET(DOXYFILE ${CMAKE_BINARY_DIR}/Doxyfile)
    CONFIGURE_FILE("${PROJECT_SOURCE_DIR}/Doxyfile.in" "${DOXYFILE}")

    ADD_CUSTOM_COMMAND(OUTPUT ${DOC_API_DIR}
                       DEPENDS ${DOXYFILE}
                       COMMAND ${CMAKE_COMMAND} ARGS -E make_directory "${DOC_API_DIR}"
                       COMMAND ${DOXYGEN_EXECUTABLE} ARGS "${DOXYFILE}"
                       WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    ADD_CUSTOM_TARGET(doc DEPENDS ${DOC_API_DIR})

    IF(ARG_AUTO_INSTALL)
        INSTALL(CODE "execute_process(COMMAND ${CMAKE_BUILD_TOOL} doc)")
        INSTALL(DIRECTORY ${DOC_API_DIR} DESTINATION "${DOC_INSTALL_DIR}")
    ENDIF()

    MESSAGE(STATUS "Generated doxygen target (doc):
   PROJECT_NAME: ${DOC_PROJECT_NAME}
   PROJECT_VERSION: ${DOC_PROJECT_VERSION}
   INSTALL_DIR: ${DOC_INSTALL_DIR}
   AUTO_INSTALL: ${ARG_AUTO_INSTALL}")
  ENDIF()

ENDFUNCTION()
