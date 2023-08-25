cmake_minimum_required(VERSION 3.16)

set(PREMAKE_BUILD_TYPES "Debug" "Release")
get_property(multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(multi_config)
  set(CMAKE_CONFIGURATION_TYPES "${PREMAKE_BUILD_TYPES}" CACHE STRING "list of supported configuration types" FORCE)
else()
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build Type of the project.")
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "${PREMAKE_BUILD_TYPES}")
  if(NOT CMAKE_BUILD_TYPE IN_LIST PREMAKE_BUILD_TYPES)
    message(FATAL_ERROR
      "Invalid build type '${CMAKE_BUILD_TYPE}'.
      CMAKE_BUILD_TYPE must be any one of the possible values:
      ${PREMAKE_BUILD_TYPES}"
    )
  endif()
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-command-line-argument")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-command-line-argument")

if (WIN32)
  set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

project("wizards-lizards")

if (EMSCRIPTEN)
  message("EMSCRIPTEN BUILD!!!")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -msimd128 -s ASYNCIFY -s TOTAL_MEMORY=67108864 -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -msimd128 -s ASYNCIFY -s TOTAL_MEMORY=67108864 -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1")


endif ()

add_subdirectory(libs/raylib)
add_subdirectory(libs/flecs)

add_subdirectory(libs/bixxy-engine)

set(BOX2D_BUILD_TESTBED OFF)
add_subdirectory(libs/box2d)



include(project/main.cmake)
