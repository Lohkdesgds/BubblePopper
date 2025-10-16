cmake_minimum_required(VERSION 3.28)

project(BubblePopper)

message(STATUS "Executing GIT submodule update and init")
execute_process( COMMAND git submodule update --init --recursive )


# ==== SETUP VARIABLES ==== #
file(GLOB_RECURSE DEPS_SRCS "deps/**/*.h")
set(DEPS_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/deps
    ${CMAKE_CURRENT_SOURCE_DIR}/deps/json/single_include
    #${CMAKE_CURRENT_SOURCE_DIR}/deps/battery-embed/include
    #${CMAKE_CURRENT_SOURCE_DIR}/deps/battery-embed/src
    #${CMAKE_CURRENT_SOURCE_DIR}/deps/SFML/include
)
set(B_PRODUCTION_MODE ON)

set(WANT_TESTS OFF CACHE BOOL "" FORCE)
set(WANT_EXAMPLES OFF CACHE BOOL "" FORCE)
set(WANT_DEMO OFF CACHE BOOL "" FORCE)
set(WANT_TOOLS OFF CACHE BOOL "" FORCE)
set(WANT_DOCS OFF CACHE BOOL "" FORCE)

set(WANT_IMAGE ON CACHE BOOL "" FORCE)
set(WANT_PRIMITIVES ON CACHE BOOL "" FORCE)
set(WANT_COLOR ON CACHE BOOL "" FORCE)
set(WANT_FONT ON CACHE BOOL "" FORCE)
set(WANT_TTF ON CACHE BOOL "" FORCE)
set(WANT_AUDIO ON CACHE BOOL "" FORCE)
set(WANT_ACODEC ON CACHE BOOL "" FORCE)
set(WANT_MEMFILE ON CACHE BOOL "" FORCE)
set(WANT_PHYSFS ON CACHE BOOL "" FORCE)
set(WANT_NATIVE_DIALOG ON CACHE BOOL "" FORCE)

# ==== SETUP DEPENDENCIES ==== #
include(FetchContent)
message(STATUS "Fetching dependencies")

# > Embedded file library
FetchContent_Declare(
  battery-embed
  GIT_REPOSITORY https://github.com/batterycenter/embed.git
  GIT_TAG        v1.2.19
)
FetchContent_MakeAvailable(battery-embed)

# > Allegro 5
FetchContent_Declare(
  allegro5
  GIT_REPOSITORY https://github.com/liballeg/allegro5.git
  GIT_TAG        5.2.10.1
)

if (MSVC)
  set(SHARED ON)
else()
  set(SHARED OFF)
endif()

FetchContent_MakeAvailable(allegro5)


# 3. Glob all addon folders under allegro5/addons/*
file(GLOB ALLEGRO_ADDON_DIRS ${allegro5_SOURCE_DIR}/addons/*)

set(ALLEGRO_ADDON_INCLUDE_DIRS "")
foreach(full_path ${ALLEGRO_ADDON_DIRS})
    if(IS_DIRECTORY "${full_path}")
        list(APPEND ALLEGRO_ADDON_INCLUDE_DIRS "${full_path}")
        message(STATUS "--> add Allegro path: ${full_path}")
    endif()
endforeach()


    message(STATUS "--> add Allegro path: ${allegro5_SOURCE_DIR}/include")
list(APPEND ALLEGRO_ADDON_INCLUDE_DIRS "${allegro5_SOURCE_DIR}/include")
    message(STATUS "--> add Allegro path: ${allegro5_BINARY_DIR}/include")
list(APPEND ALLEGRO_ADDON_INCLUDE_DIRS "${allegro5_BINARY_DIR}/include")

list(REMOVE_DUPLICATES ALLEGRO_ADDON_INCLUDE_DIRS)


# ==== SET CXX STANDARD ==== #
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ==== ENABLING TESTING ==== #
enable_testing()

# ==== ADDING PROJECTS ==== #
add_subdirectory(main)
#add_subdirectory(test) # No need for now, this is just a test project