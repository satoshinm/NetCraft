cmake_minimum_required(VERSION 2.8)

project(craft)

FILE(GLOB SOURCE_FILES src/*.c)

if (EMSCRIPTEN)
    add_executable(
        craft
        ${SOURCE_FILES}
        deps/lodepng/lodepng.c
        deps/miniz/miniz.c
        deps/noise/noise.c
        deps/tinycthread/tinycthread.c)
    # Generate HTML file wrapper in addition to .js
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
else ()
    FILE(COPY textures DESTINATION .)
    FILE(COPY shaders DESTINATION .)
    add_executable(
        craft
        ${SOURCE_FILES}
        deps/glew/src/glew.c
        deps/lodepng/lodepng.c
        deps/miniz/miniz.c
        deps/noise/noise.c
        deps/sqlite/sqlite3.c
        deps/tinycthread/tinycthread.c)
endif ()

add_definitions(-std=c99 -O3)
add_definitions(-DLODEPNG_NO_COMPILE_ZLIB)

set_property(SOURCE ${SOURCE_FILES} PROPERTY COMPILE_FLAGS "-Wall -Wextra -pedantic -Wno-unused-parameter -Wno-missing-field-initializers -Wno-sign-compare -Wno-missing-prototypes -Wno-reserved-id-macro -Wno-documentation -Wno-conversion -Wno-sign-conversion -Wno-float-equal -Wno-conversion -Wno-double-promotion -Wno-padded -Wno-shadow -Wno-disabled-macro-expansion -Wno-bad-function-cast -Wno-documentation-unknown-command -Wno-date-time -Wno-cast-qual")

if ($ENV{CIRCLECI})
    add_definitions(-DBUILD_NUM="$ENV{CIRCLE_BUILD_NUM}")
    add_definitions(-DBUILD_COMMIT="$ENV{CIRCLE_SHA1}")
endif()

if (DEBUG)
    add_definitions("-fsanitize=address -fsanitize=undefined -g")
    set_property(TARGET craft APPEND_STRING PROPERTY LINK_FLAGS "-fsanitize=address -fsanitize=undefined -g")
endif ()

if (EMSCRIPTEN)
    add_definitions(-DNO_CRAFT_AUTH)

    # Emscripten default is GLFW 2.x but we use GLFW 3.x, also include data files
    set_target_properties(craft PROPERTIES LINK_FLAGS "-s USE_GLFW=3 --pre-js ../src/pre.js --shell-file ../src/shell.html --embed-file ../shaders/ -s TOTAL_MEMORY=33554432 --embed-file ../textures/ -s EXPORTED_FUNCTIONS=\"['_main','_is_typing']\"")

    if (CMAKE_RELEASE_TYPE EQUAL "Release")
        set_property(TARGET craft APPEND_STRING PROPERTY LINK_FLAGS " -O3")
    endif ()

    if (WASM)
        set_property(TARGET craft APPEND_STRING PROPERTY LINK_FLAGS " -s WASM=1")
    endif ()
endif ()

if (NO_CRAFT_AUTH)
    add_definitions(-DNO_CRAFT_AUTH)
endif ()

# Emscripten includes its own GLFW and GLEW ports
if (NOT EMSCRIPTEN)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
    add_subdirectory(deps/glfw)
    include_directories(deps/glew/include)
    include_directories(deps/glfw/include)
    include_directories(deps/sqlite)
endif ()

include_directories(deps/lodepng)
include_directories(deps/miniz)
include_directories(deps/noise)
include_directories(deps/tinycthread)

if(MINGW OR MSVC)
    set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH}
        "C:/Program Files/CURL/lib" "C:/Program Files (x86)/CURL/lib")
    set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH}
        "C:/Program Files/CURL/include" "C:/Program Files (x86)/CURL/include")
endif()

if (NOT EMSCRIPTEN)
    find_package(CURL REQUIRED)
    include_directories(${CURL_INCLUDE_DIR})
else ()
    set(CURL_LIBRARIES "")
endif ()

if(APPLE OR EMSCRIPTEN)
    target_link_libraries(craft glfw
        ${GLFW_LIBRARIES} ${CURL_LIBRARIES})
endif()

if(UNIX AND NOT EMSCRIPTEN)
    find_package(OpenGL REQUIRED)
    target_link_libraries(craft dl glfw
        ${OPENGL_gl_LIBRARY}
        ${GLFW_LIBRARIES} ${CURL_LIBRARIES})
endif()

if(MINGW OR MSVC)
    find_package(OpenGL REQUIRED)
    target_link_libraries(craft ws2_32.lib glfw
        ${OPENGL_gl_LIBRARY}
        ${GLFW_LIBRARIES} ${CURL_LIBRARIES})
endif()
