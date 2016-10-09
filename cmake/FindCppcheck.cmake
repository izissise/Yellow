# - Try to find the cppcheck utility
# Will define:
#
# CPPCHECK_EXECUTABLE - cppcheck executable
# CPPCHECK_VERSION - version of cppcheck
#
# Possible hints:
# CPPCHECK_ROOT - root directory of the mongodb installation
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

INCLUDE(FindPackageHandleStandardArgs)

# find server executable
FIND_PROGRAM(CPPCHECK_EXECUTABLE cppcheck
             HINTS ${CPPCHECK_ROOT}/bin
                   ${CPPCHECK_ROOT})

# get the version
SET(CPPCHECK_VERSION "")
IF(CPPCHECK_EXECUTABLE)
  EXECUTE_PROCESS(COMMAND ${CPPCHECK_EXECUTABLE} --version OUTPUT_VARIABLE CPPCHECK_VERSION_OUT)
  STRING(REGEX MATCH "[0-9]\\.[0-9]+" CPPCHECK_VERSION "${CPPCHECK_VERSION_OUT}")
ENDIF()

FIND_PACKAGE_HANDLE_STANDARD_ARGS(Cppcheck DEFAULT_MSG CPPCHECK_EXECUTABLE CPPCHECK_VERSION)

# only visible in advanced view
MARK_AS_ADVANCED(CPPCHECK_EXECUTABLE CPPCHECK_VERSION)
