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

find_package(PkgConfig REQUIRED)
pkg_search_module(PC_Allegro5 QUIET allegro-5)

set(Allegro5_FOUND ${PC_Allegro5_FOUND})

find_path(Allegro5_INCLUDE_DIR
  NAMES "allegro5/allegro.h" "allegro5/allegro5.h"
  PATHS ${PC_Allegro5_INCLUDE_DIRS}
)
find_library(Allegro5_LIBRARY
  NAMES allegro
  PATH ${PC_Allegro5_LIBRARY_DIRS}
)
set(Allegro5_VERSION ${PC_Allegro5_VERSION})

if(Allegro5_FOUND)
  set(Allegro5_LIBRARIES ${Allegro5_LIBRARY})
  set(Allegro5_INCLUDE_DIRS ${Allegro5_INCLUDE_DIR})
  set(Allegro5_DEFINITIONS ${PC_Allegro5_CFLAGS_OTHER})
endif()

if(Allegro5_FOUND AND NOT TARGET Allegro::Allegro)
  add_library(Allegro::Allegro UNKNOWN IMPORTED)
  set_target_properties(Allegro::Allegro PROPERTIES
    IMPORTED_LOCATION "${Allegro5_LIBRARIES}" 
    INTERFACE_COMPILE_OPTIONS "${Allegro5_DEFINITIONS}"
    INTERFACE_INCLUDE_DIRECTORIES "${Allegro5_INCLUDE_DIRS}"
  )
endif()

# Seatch Allegro5 Add-Ons and include them in the variables
# will set Allegro5_FOUND to 0 if any Add-on is not found.
foreach(component ${Allegro5_FIND_COMPONENTS})
  pkg_search_module(PC_Allegro5_${component} QUIET allegro_${component}-5)
  if("${PC_Allegro5_${component}_FOUND}")
    find_library(Allegro5_${component}_LIBRARY
      NAMES allegro_${component} allegro_${component}-5
      PATH "${PC_Allegro5_${component}_LIBRARY_DIRS}"
    )
    
    set(Allegro5_${component}_FOUND "${PC_Allegro5_${component}_FOUND}")
    
    if(Allegro5_${component}_FOUND AND NOT TARGET Allegro::${component})
      add_library(Allegro::${component} UNKNOWN IMPORTED)
      set_target_properties(Allegro::${component} PROPERTIES
        IMPORTED_LOCATION "${Allegro5_${component}_LIBRARY}" 
        INTERFACE_COMPILE_OPTIONS "${PC_Allegro55_${component}_CFLAGS_OTHER}"
        INTERFACE_INCLUDE_DIRECTORIES "${Allegro5_INCLUDE_DIR}"
      )
    endif()
    
    mark_as_advanced(Allegro5_${component}_INCLUDE_DIR Allegro5_${component}_LIBRARY)
  else()
    message([FATAL_ERROR] "The Allegro5 ${component} Add-On was not found!")
    break()
  endif()
endforeach()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Allegro5
  FOUND_VAR Allegro5_FOUND
  REQUIRED_VARS
    Allegro5_LIBRARY
    Allegro5_INCLUDE_DIR
  VERSION_VAR Allegro5_VERSION
  HANDLE_COMPONENTS
)

mark_as_advanced(Allegro5_FOUND Allegro5_INCLUDE_DIR Allegro5_VERSION Allegro5_LIBRARY)