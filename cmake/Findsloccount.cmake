# - Find sloccount
# Will define:
#
# SLOCCOUNT_EXECUTABLE - the sloccount binary
#
# Uses:
#
# SLOCCOUNT_ROOT - additional search hint for the binary
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

FIND_PROGRAM(SLOCCOUNT_EXECUTABLE sloccount HINTS ${SLOCCOUNT_ROOT})

FIND_PACKAGE_HANDLE_STANDARD_ARGS(sloccount DEFAULT_MSG SLOCCOUNT_EXECUTABLE)

# only visible in advanced view
MARK_AS_ADVANCED(SLOCCOUNT_EXECUTABLE)
