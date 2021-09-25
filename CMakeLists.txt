# Copyright (c) 2021-present Sparky Studios. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.20)

set(PROJECT_N pixel-ui)
project(${PROJECT_N} VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(ALLEGRO_COMPONENTS
  dialog
  font
  image
  primitives
  ttf
)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

# Search fo Allgro5 libraries
find_package(Allegro5 COMPONENTS ${ALLEGRO_COMPONENTS} REQUIRED)

file(GLOB_RECURSE SRCS
  ${PROJECT_SOURCE_DIR}/src/**.cpp
  ${PROJECT_SOURCE_DIR}/src/**/*.cpp
  ${PROJECT_SOURCE_DIR}/src/**/*.h
  ${PROJECT_SOURCE_DIR}/src/*.h
  ${PROJECT_SOURCE_DIR}/include/**/*.h
  ${PROJECT_SOURCE_DIR}/include/*.h
)

if (DEFINED COMPILE_MODE)
  add_definitions(-DPI_COMPILE_${COMPILE_MODE})

  if(COMPILE_MODE STREQUAL "SHARED")
    add_library(${PROJECT_N} SHARED ${SRCS})
  elseif(COMPILE_MODE STREQUAL "STATIC")
    add_library(${PROJECT_N} STATIC ${SRCS})
  else()
    message(ERROR "Bad compile mode value.")
  endif()
endif()

target_link_libraries(${PROJECT_N} PRIVATE Allegro::Allegro)
foreach(component ${ALLEGRO_COMPONENTS})
  target_link_libraries(${PROJECT_N} PRIVATE Allegro::${component})
endforeach()

target_include_directories(${PROJECT_N}
  PRIVATE ${Allegro_INCLUDE_DIR}
  PUBLIC include src
)

file(GLOB PI_SAMPLES_DIRECTORIES samples/**)

add_subdirectory(${PI_SAMPLES_DIRECTORIES})
