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

cmake_minimum_required(VERSION 3.24.2)

project(captive VERSION 1.0.0)

# Global compiler options
add_compile_options(-Wall)

add_executable(${CMAKE_PROJECT_NAME})
target_compile_features(${CMAKE_PROJECT_NAME} PUBLIC cxx_std_11)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
  src/main.cc
  src/game.cc
  src/common.cc
  src/captive.cc
)
