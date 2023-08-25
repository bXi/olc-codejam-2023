cmake_minimum_required(VERSION 3.12)  # Set your required minimum CMake version

project("bixxy-engine")  # Set your project name

# Add any required subdirectories or additional targets

# Add the content from the provided "engine.cmake" file
add_library("bixxy-engine" STATIC
        "audio/audiohandler.cpp"
        "audio/audiohandler.h"
        "fonts/fonthandler.cpp"
        "fonts/fonthandler.h"
        "input/inputhandler.cpp"
        "input/inputhandler.h"
        "log/loghandler.cpp"
        "log/loghandler.h"
        "settings/mini.h"
        "settings/settingshandler.cpp"
        "settings/settingshandler.h"
        "state/state.h"
        "texture/texturehandler.cpp"
        "texture/texturehandler.h"
        "utils/easings.h"
        "utils/lerp.cpp"
        "utils/lerp.h"
        "utils/quadtree.h"
        "utils/vectors.h"
        "utils/helpers.h"
        "utils/helpers.cpp"
        configuration/configuration.h
        state/basestate.h

)

target_include_directories("bixxy-engine" PUBLIC
        ${PROJECT_SOURCE_DIR} "../../libs/raylib/src"
        ${PROJECT_SOURCE_DIR} "../../libs/bixxy-engine"
        ${PROJECT_SOURCE_DIR} "../../libs/box2d/include"
        ${PROJECT_SOURCE_DIR} "../../libs/flecs"
        ${PROJECT_SOURCE_DIR} "../../libs/raylib/src/external/glfw/include"
        ${PROJECT_SOURCE_DIR} "project/src"
)

set_target_properties("bixxy-engine" PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        POSITION_INDEPENDENT_CODE False
        INTERPROCEDURAL_OPTIMIZATION False
)

if (CMAKE_BUILD_TYPE STREQUAL Release)
    set(OUTPUT_DIR "bin/Release")

    target_compile_definitions("bixxy-engine" PRIVATE
            "NDEBUG"
    )

    if (EMSCRIPTEN)
                target_compile_options("bixxy-engine" PRIVATE
                $<$<COMPILE_LANGUAGE:C>:-O3>
                $<$<COMPILE_LANGUAGE:CXX>:-O3>
                $<$<COMPILE_LANGUAGE:CXX>:-std=c++20>
        )
    else()
        target_compile_options("bixxy-engine" PRIVATE
                $<$<COMPILE_LANGUAGE:C>:-m64>
                $<$<COMPILE_LANGUAGE:C>:-O3>
                $<$<COMPILE_LANGUAGE:CXX>:-m64>
                $<$<COMPILE_LANGUAGE:CXX>:-O3>
                $<$<COMPILE_LANGUAGE:CXX>:-std=c++20>
        )
    endif()

else ()

    set(OUTPUT_DIR "bin/Debug")
    target_compile_definitions("bixxy-engine" PRIVATE
            "DEBUG"
    )
    target_compile_options("bixxy-engine" PRIVATE
            $<$<COMPILE_LANGUAGE:C>:-m64>
            $<$<COMPILE_LANGUAGE:C>:-g>
            $<$<COMPILE_LANGUAGE:CXX>:-m64>
            $<$<COMPILE_LANGUAGE:CXX>:-g>
            $<$<COMPILE_LANGUAGE:CXX>:-std=c++20>
    )
endif ()

set_target_properties("bixxy-engine" PROPERTIES
        OUTPUT_NAME "bixxy-engine"
        ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR}
        LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR}
        RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
)
