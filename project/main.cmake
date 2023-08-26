add_executable("main"
        "project/resource/main.rc"
        "project/resource/mainIcon.ico"
        "project/resource/resource.h"
        "project/src/components/aicontroller.h"
        "project/src/components/collision.h"
        "project/src/components/damage.h"
        "project/src/components/health.h"
        "project/src/components/owner.h"
        "project/src/components/playerclass.cpp"
        "project/src/components/playerclass.h"
        "project/src/components/playerindex.h"
        "project/src/components/playerinput.h"
        "project/src/components/radius.h"
        "project/src/components/render2d.cpp"
        "project/src/components/render2d.h"
        "project/src/components/rigidbody2d.h"
        "project/src/components/sprite.h"
        "project/src/components/type.h"
        "project/src/components/weapon.h"
        "project/src/ecs.h"
        "project/src/entities/fireball.cpp"
        "project/src/entities/fireball.h"
        "project/src/entities/lizard.cpp"
        "project/src/entities/lizard.h"
        "project/src/entities/physicsobject.h"
        "project/src/entities/player.cpp"
        "project/src/entities/player.h"
        "project/src/main.cpp"
        "project/src/main.h"
        "project/src/map/levelmanager.cpp"
        "project/src/map/levelmanager.h"
        "project/src/states/game/game.cpp"
        "project/src/states/game/game.h"
        "project/src/states/intro/intro.cpp"
        "project/src/states/intro/intro.h"
        "project/src/ui/ui.cpp"
        "project/src/ui/ui.h"
        "project/src/world/world.cpp"
        "project/src/world/world.h"
        "project/src/entities/physicsobject.cpp"
)

set_target_properties("main" PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        POSITION_INDEPENDENT_CODE False
        INTERPROCEDURAL_OPTIMIZATION False
)

if (CMAKE_BUILD_TYPE STREQUAL Release)
    set(OUTPUT_DIR "../../bin/Release")

    target_compile_definitions("main" PRIVATE
            "NDEBUG"
    )

    if (EMSCRIPTEN)
        set(OUTPUT_DIR "../../bin/Web")

        target_compile_options("main" PRIVATE
                $<$<COMPILE_LANGUAGE:C>:-O0>
                $<$<COMPILE_LANGUAGE:CXX>:-O0>
                $<$<COMPILE_LANGUAGE:CXX>:-std=c++20>
        )


        set_target_properties("main" PROPERTIES LINK_FLAGS "-s FORCE_FILESYSTEM=1 -s TOTAL_MEMORY=67108864 -sSTACK_SIZE=5MB -s ASYNCIFY -s DEMANGLE_SUPPORT=1 -s EXPORTED_FUNCTIONS=\"[_malloc,_main]\" --shell-file ${CMAKE_SOURCE_DIR}/project/shell.html --preload-file ${CMAKE_SOURCE_DIR}/project/assets/@assets --bind")

        set(CMAKE_EXECUTABLE_SUFFIX ".html") # This line is used to set your executable to build with the emscripten html template so taht you can directly open it.

    else ()
        target_compile_options("main" PRIVATE
                $<$<COMPILE_LANGUAGE:C>:-m64>
                $<$<COMPILE_LANGUAGE:C>:-O3>
                $<$<COMPILE_LANGUAGE:CXX>:-m64>
                $<$<COMPILE_LANGUAGE:CXX>:-O3>
                $<$<COMPILE_LANGUAGE:CXX>:-std=c++20>
        )
    endif ()

    target_link_options("main" PRIVATE -mwindows)
else ()
    set(OUTPUT_DIR "../../bin/Debug")

    target_compile_definitions("main" PRIVATE
            "DEBUG"
    )

    target_compile_options("main" PRIVATE
            $<$<COMPILE_LANGUAGE:C>:-m64>
            $<$<COMPILE_LANGUAGE:C>:-g>
            $<$<COMPILE_LANGUAGE:CXX>:-m64>
            $<$<COMPILE_LANGUAGE:CXX>:-g>
            $<$<COMPILE_LANGUAGE:CXX>:-std=c++20>
    )
endif ()

if (WIN32)
    target_compile_definitions("main" PRIVATE
            "_WIN32"
    )
endif ()

set_target_properties("main" PROPERTIES
        OUTPUT_NAME "main"
        ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR}
        LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR}
        RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
)
target_link_directories("main" PRIVATE
        ${PROJECT_SOURCE_DIR} "/" ${OUTPUT_DIR}
)

set(FLECS_SHARED off)
target_link_libraries("main" flecs::flecs_static)

add_dependencies("main"
        "raylib"
        "box2d"
        "bixxy-engine"
)

target_include_directories("main" PRIVATE
        ${PROJECT_SOURCE_DIR} "project/src"
        ${PROJECT_SOURCE_DIR} "libs/bixxy-engine"
        ${PROJECT_SOURCE_DIR} "libs/raylib/src"
        ${PROJECT_SOURCE_DIR} "libs/flecs/include"
        ${PROJECT_SOURCE_DIR} "libs/raylib/src/external/glfw/include"
        ${PROJECT_SOURCE_DIR} "libs/box2d/include"
        ${PROJECT_SOURCE_DIR} "libs/box2d/include"
)

target_link_libraries("main"
        "raylib"
        "box2d"
        "bixxy-engine"
        nlohmann_json::nlohmann_json
)
if (WIN32)
    target_link_libraries("main"
            "winmm"
            "opengl32"
            "kernel32"
            "gdi32"
    )

    if (CMAKE_BUILD_TYPE STREQUAL Debug)
        target_link_libraries("main"
                "wsock32"
                "ws2_32"
        )
    endif ()
endif ()

if (NOT EMSCRIPTEN)
    file(COPY project/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../../bin/${CMAKE_BUILD_TYPE})
endif ()