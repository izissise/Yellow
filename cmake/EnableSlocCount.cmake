# - Creates a target to calculate the source lines of code
#
# If the sloccount executable is found, a special target will be created
# that counts the source lines of code for specified folders. To create this
# target, use this function that can also be used if sloccount was not found:
#
# ENABLE_SLOCCOUNT(FOLDERS Folder ... [RESULT_FILE file])
#
# Results will be given on the console and stored in a file. The default for
# this file is CMAKE_BINARY_DIR/sloccount.sc.
#
# Copyright (C) 2011 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
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

INCLUDE(ParseArguments)
FIND_PACKAGE(sloccount)

FUNCTION(ENABLE_SLOCCOUNT)

    # argument parsing
    PARSE_ARGUMENTS(ARG "FOLDERS;RESULT_FILE" "" ${ARGN})

    SET(RESULT_FILE "${CMAKE_BINARY_DIR}/sloccount.sc")
    SET(SLOCCOUNT_COMMAND_FILE "${CMAKE_BINARY_DIR}/sloccount.cmake")

    # argument parsing
    LIST(LENGTH ARG_FOLDERS FOLDERS_LENGTH)
    IF(FOLDERS_LENGTH EQUAL 0)
        MESSAGE(SEND_ERROR "No folders given for sloccount target")
    ENDIF()

    LIST(LENGTH ARG_RESULT_FILE RESULT_FILE_LENGTH)
    IF(RESULT_FILE_LENGTH GREATER 1)
        MESSAGE(SEND_ERROR "Mutiple result file arguments given for sloccount target.")
    ENDIF()
    IF(RESULT_FILE_LENGTH EQUAL 1)
        SET(RESULT_FILE ${ARG_RESULT_FILE})
    ENDIF()

    IF(SLOCCOUNT_FOUND)

        MESSAGE(STATUS "Creating sloccount target for folders: ${ARG_FOLDERS} with result file ${RESULT_FILE}")

        # sloccount cannot write directly to a file so the execution needs to
        # be wrapped in a cmake file that generates the file output
        FILE(WRITE ${SLOCCOUNT_COMMAND_FILE}
             "EXECUTE_PROCESS(COMMAND \"${SLOCCOUNT_EXECUTABLE}\" --details ")
        FOREACH(FOLDER ${ARG_FOLDERS})
            FILE(APPEND ${SLOCCOUNT_COMMAND_FILE} " \"${FOLDER}\" ")
        ENDFOREACH()
        FILE(APPEND ${SLOCCOUNT_COMMAND_FILE}
             " OUTPUT_FILE \"${RESULT_FILE}\" WORKING_DIRECTORY \"${CMAKE_CURRENT_SOURCE_DIR}\" ERROR_QUIET)\n")
        FILE(APPEND ${SLOCCOUNT_COMMAND_FILE}
             "FILE(READ \"${RESULT_FILE}\" SLOCCOUNT_OUT)\n")
        FILE(APPEND ${SLOCCOUNT_COMMAND_FILE}
             "MESSAGE(\"\${SLOCCOUNT_OUT}\")\n")

        ADD_CUSTOM_TARGET(sloccount
                          ${CMAKE_COMMAND} -P ${SLOCCOUNT_COMMAND_FILE}
                          COMMENT "Generating sloccount report")

    ENDIF()

ENDFUNCTION()
