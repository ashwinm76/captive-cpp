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
