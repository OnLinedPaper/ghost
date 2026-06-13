# first cmake file! super useful stuff but god it's confusing. has already
# been revised several times and will likely continue to be changed.
#
# useful links:
# - https://www.studyplan.dev/sdl3/sdl3-cmake

# rebuild build dir and set prefix:
# rm -rf build/ && cmake -B build

# per tutorial recommendations, i'm adding a note why i picked this version.
# it's literally just because this was the version installed on my linux
# machine, and i wanted parity across systems.
cmake_minimum_required(VERSION 3.22.1)

# high-level name for project
project(
  ukagaka
  VERSION 0.0.1
  DESCRIPTION "first attempt at cross-platform \"ghost\" with transparency"
  LANGUAGES CXX
)

# enforce modern standard
set(CMAKE_CXX_STANDARD 20) #arbitrary
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# stick the binaries and libraries in the same folder
set(
  CMAKE_RUNTIME_OUTPUT_DIRECTORY
  "."
)
set(
  CMAKE_LIBRARY_OUTPUT_DIRECTORY
  "."
)

# show cmake where SDL is installed
SET(SDL3_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/lib/cmake/SDL3/)
SET(SDL3_image_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/lib/cmake/SDL3_image)

# get sdl
find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)

add_compile_options(-Wall -Wextra -Wpedantic -O0 -g)

# on windows only - dont show a cmd on startup
IF (WIN32)
  add_link_options(-Wl,-subsystem,windows)
ENDIF()



# main program and name of binary that gets built
add_executable(ghost)
target_sources(ghost
  PRIVATE
    src/main.cpp
)

# special section aiming for cross-compatibility with mac and linux
if(APPLE)
  set_target_properties(ghost PROPERTIES
    INSTALL_RPATH "@executable_path;@loader_path"
    BUILD_WITH_INSTALL_RPATH TRUE
    MACOSX_RPATH TRUE
)
elseif(UNIX)
  set_target_properties(ghost PROPERTIES
    INSTALL_RPATH "$ORIGIN"
    BUILD_WITH_INSTALL_RPATH TRUE
)
endif()

#---- internal libs -----------------------------------------------------------

# -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -
# top-level project
target_link_libraries(ghost PRIVATE engine)
target_link_libraries(ghost PRIVATE window)
target_link_libraries(ghost PRIVATE  image)
target_link_libraries(ghost PRIVATE script)
target_link_libraries(ghost PRIVATE   time)

# -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -
# main engine loop
add_library(engine)
target_sources(engine
  PRIVATE
    # implementation - no one needs to see this
    src/engine.cpp

  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      # tells the compiler where to look for these (like -I)
      # (note that this allows us to omit include/ in the #include, i.e. i
      # can do #include "engine.h" instead of #include "include/engine.h")
      include
    FILES
      # interface - everyone needs to see this
      include/engine.h
)

# -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -
# window manager
add_library(window)
target_sources(window
  PRIVATE
    src/window.cpp

  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      include
    FILES
      include/window.h
)

# -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -
# images
add_library(image)
target_sources(image
  PRIVATE
    src/image.cpp

  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      include
    FILES
      include/image.h
)

# -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -
# animation scripts
add_library(script)
target_sources(script
  PRIVATE
    src/script.cpp

  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      include
    FILES
      include/script.h
)

# -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -
# timekeeping
add_library(time)
target_sources(time
  PRIVATE
    src/timekeep.cpp
  
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      include
    FILES
      include/timekeep.h
)

#---- external libs -----------------------------------------------------------

# -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -
# include SDL libs for the subcomponents which need them
target_include_directories(engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external/include)
target_include_directories(window PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external/include)
target_include_directories( image PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external/include)

# -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -
# link SDL at the top level to ghost as a whole
target_link_libraries(ghost PRIVATE -L${CMAKE_CURRENT_SOURCE_DIR}/external/lib)
if (WIN32)
  target_link_libraries(ghost PRIVATE -L${CMAKE_CURRENT_SOURCE_DIR}/external/lib/SDL3main)
endif()

find_library(SDL3
  NAMES sdl3
  HINTS "./external/lib"
)
target_link_libraries(ghost PRIVATE SDL3)

find_library(SDL3_image
  NAMES sdl3_image
  HINTS "./external/lib"
)
target_link_libraries(ghost PRIVATE SDL3_image)


# -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -
#TODO: old stuff i'm not getting rid of until i'm SURE i don't need it anymore
#target_include_directories(engine PRIVATE lib/SDL-release-3.4.8/include)
#add_subdirectory (lib/SDL-release-3.4.8)
#target_include_directories(engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external/lib/cmake)
#target_link_libraries(engine PRIVATE -I${CMAKE_CURRENT_SOURCE_DIR}/include)
#target_link_libraries(ghost PRIVATE -lmingw32)


#---- move libs where they can be found ---------------------------------------

# -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -
#TODO: old stuff i'm not getting rid of until i'm SURE i don't need it anymore
#add_custom_command(
 # TARGET engine POST_BUILD COMMAND
  #${CMAKE_COMMAND} -E copy_if_different
   # "$<TARGET_FILE:SDL3>"
    #"$<TARGET_FILE_DIR:engine>"
 # VERBATIM
#)
