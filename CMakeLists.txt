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

set(PROJECT_N Pixel)
project(${PROJECT_N} VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (NOT DEFINED COMPILE_MODE)
  set(COMPILE_MODE "STATIC")
endif()

set(ALLEGRO_COMPONENTS
  dialog
  font
  image
  primitives
  ttf
)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

# Search fo Allgro5 libraries
find_package(unofficial-allegro5 COMPONENTS ${ALLEGRO_COMPONENTS} REQUIRED)

file(GLOB_RECURSE SRCS
  ${PROJECT_SOURCE_DIR}/src/**.cpp
  ${PROJECT_SOURCE_DIR}/src/**/*.cpp
  ${PROJECT_SOURCE_DIR}/src/**/*.h
  ${PROJECT_SOURCE_DIR}/src/*.h
  ${PROJECT_SOURCE_DIR}/include/**/*.h
  ${PROJECT_SOURCE_DIR}/include/*.h
)

add_definitions(-DPI_COMPILE_${COMPILE_MODE})
add_definitions(-DALLEGRO_${COMPILE_MODE}LINK)

add_library(${PROJECT_N} ${COMPILE_MODE} ${SRCS})

target_link_libraries(${PROJECT_N} PRIVATE unofficial-allegro5::allegro)
foreach(component ${ALLEGRO_COMPONENTS})
  target_link_libraries(${PROJECT_N} PRIVATE unofficial-allegro5::allegro_${component})
endforeach()

find_path(Allegro5_INCLUDE_DIR
  NAMES "allegro5/allegro.h" "allegro5/allegro5.h"
)

target_include_directories(${PROJECT_N}
  PRIVATE ${Allegro5_INCLUDE_DIR}
  PUBLIC include src
)

file(GLOB PI_SAMPLES_DIRECTORIES samples/**)

add_subdirectory(${PI_SAMPLES_DIRECTORIES})
