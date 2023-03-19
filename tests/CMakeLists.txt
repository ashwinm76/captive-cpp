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

add_executable(test_fixture)
target_sources(test_fixture PRIVATE test_fixture.cc)
target_link_libraries(test_fixture ${CMAKE_PROJECT_NAME}_lib)

add_test(NAME TestWin1 COMMAND test_fixture ${CMAKE_CURRENT_LIST_DIR}/win1.txt)

foreach(X RANGE 1 16)
  add_test(NAME TestLose${X} COMMAND test_fixture ${CMAKE_CURRENT_LIST_DIR}/lose${X}.txt)
endforeach()