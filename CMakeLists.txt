cmake_minimum_required(VERSION 3.20)
project(JohnnyCastaway C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(JC_STATIC_SDL "Link SDL2 statically (default ON for Windows)" ${WIN32})

if(APPLE)
  list(APPEND CMAKE_PREFIX_PATH "/usr/local" "/opt/homebrew" "/usr/local/opt/sdl2" "/opt/homebrew/opt/sdl2" "/opt/local")
endif()

find_package(SDL2 REQUIRED CONFIG)

# Pick SDL target
if(JC_STATIC_SDL AND TARGET SDL2::SDL2-static)
  set(JC_SDL_TARGET SDL2::SDL2-static)
else()
  set(JC_SDL_TARGET SDL2::SDL2)
endif()

set(ENGINE_SOURCES
  engine/ads.c engine/bench.c engine/calcpath.c engine/config.c engine/dump.c
  engine/events.c engine/graphics.c engine/island.c engine/resource.c
  engine/respath.c
  engine/sound.c engine/story.c engine/ttm.c engine/uncompress.c engine/utils.c
  engine/walk.c
  engine/xbrz/xbrz.cpp engine/xbrz/xbrz_bridge.cpp
)

add_library(jcengine STATIC ${ENGINE_SOURCES})
target_link_libraries(jcengine PUBLIC ${JC_SDL_TARGET})
target_include_directories(jcengine PUBLIC engine engine/xbrz)
# The engine includes <SDL2/SDL.h>: also expose the parent of SDL's include dir.
get_target_property(_sdl_inc ${JC_SDL_TARGET} INTERFACE_INCLUDE_DIRECTORIES)
foreach(_d IN LISTS _sdl_inc)
  if(NOT _d MATCHES "\\$<")
    target_include_directories(jcengine PUBLIC "${_d}/..")
  endif()
endforeach()
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(jcengine PRIVATE -Wall)
endif()

# --- Player / screensaver executable -------------------------------------------------
add_executable(johnny engine/jc_reborn.c)
target_link_libraries(johnny PRIVATE jcengine)

if(WIN32)
  target_sources(johnny PRIVATE windows/JohnnyCastaway.rc)
  if(TARGET SDL2::SDL2main)
    target_link_libraries(johnny PRIVATE SDL2::SDL2main)
  endif()
  set_target_properties(johnny PROPERTIES WIN32_EXECUTABLE ON OUTPUT_NAME "JohnnyCastaway" SUFFIX ".scr")
  if(MINGW)
    target_link_options(johnny PRIVATE -static -static-libgcc -static-libstdc++)
  endif()
endif()

if(APPLE)
  enable_language(OBJC)
  add_library(JohnnyCastawaySaver MODULE
    macos/JohnnyCastawayView.m
  )
  target_link_libraries(JohnnyCastawaySaver PRIVATE
    jcengine
    "-framework ScreenSaver"
    "-framework Cocoa"
    "-framework CoreGraphics"
  )
  set_target_properties(JohnnyCastawaySaver PROPERTIES
    BUNDLE TRUE
    OUTPUT_NAME "JohnnyCastaway"
    BUNDLE_EXTENSION "saver"
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/macos/Info.plist"
  )
endif()

# --- Tools ---------------------------------------------------------------------------
add_executable(extract_walk_data engine/extract_walk_data.c)

install(TARGETS johnny RUNTIME DESTINATION bin)
