#
# Captive - A C++ port of the BASIC type-in game 'captive'
# Copyright (C) 2022  Ashwin Menon

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#

add_executable(test_win1)
target_sources(test_win1 PRIVATE test_win1.cc)
target_link_libraries(test_win1 ${CMAKE_PROJECT_NAME}_lib)

add_executable(test_lose1)
target_sources(test_lose1 PRIVATE test_lose1.cc)
target_link_libraries(test_lose1 ${CMAKE_PROJECT_NAME}_lib)

add_test(NAME TestWin1 COMMAND test_win1)
add_test(NAME TestLose1 COMMAND test_lose1)
