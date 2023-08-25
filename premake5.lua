workspace "wizards-lizards"
	configurations { "Debug", "Release" }
	platforms { "x64"}
	defaultplatform "x64"
	startproject "main"


	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

	filter { "platforms:x64" }
		architecture "x86_64"

	targetdir "bin/%{cfg.buildcfg}/"


project "raylib"
	filter "configurations:Debug OR Release"
		kind "StaticLib"

	filter "action:vs*"
		defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
		links {"winmm"}

	filter{}

  location "libs/raylib-build"
	language "C"
	targetdir "bin/%{cfg.buildcfg}"

	includedirs { "libs/raylib/src", "libs/raylib/src/external/glfw/include"}
	vpaths
	{
		["Header Files"] = { "libs/raylib/src/**.h"},
		["Source Files/*"] = {"libs/raylib/src/**.c"},
	}
	files {"libs/raylib/src/*.h", "libs/raylib/src/*.c"}

	filter { "platforms:ARM" }
		defines {"PLATFORM_DESKTOP", "GRAPHICS_API_OPENGL_21"}
	filter { "platforms:x64" }
		defines{"PLATFORM_DESKTOP", "GRAPHICS_API_OPENGL_33"}

project "box2d"
	kind "StaticLib"

	filter{}

	location "./libs/box2d-build"
	language "C++"
	cppdialect "C++20"
	targetdir "bin/%{cfg.buildcfg}"

	includedirs { "libs/box2d/include" }
	vpaths
	{
		["Header Files"] = { "libs/box2d/include/**.h"},
		["Source Files"] = {"libs/box2d/src/**.cpp"},
	}
	files { "libs/box2d/src/**.cpp", "libs/box2d/include/**.h" }

project "bixxy-engine"
	kind "StaticLib"

	filter{}

	location "./libs/bixxy-engine/"
	language "C++"
	cppdialect "C++20"
	targetdir "bin/%{cfg.buildcfg}"

	includedirs {
		"libs/raylib/src", "libs/bixxy-engine",
		"libs/box2d/include",
		"libs/flecs",
		"libs/raylib/src/external/glfw/include" }
	vpaths
	{
		["Header Files"] = { "libs/bixxy-engine/**.h"},
		["Source Files"] = {"libs/bixxy-engine/**.cpp"},
	}
	files {"libs/bixxy-engine/**.h", "libs/bixxy-engine/**.cpp" }

project "flecs"
	kind "StaticLib"

	filter{}

	location "./libs/flecs-build"
	language "C++"
	cppdialect "C++14"
	targetdir "bin/%{cfg.buildcfg}"

	includedirs { "libs/flecs/include" }
	vpaths
	{
		["Header Files"] = { "libs/flecs/include/**.h", "libs/flecs/src/**.h"},
		["Source Files"] = { "libs/flecs/src/**.c" },
	}
	files { "libs/flecs/flecs.h", "libs/flecs/flecs.c" }
  --files { "libs/flecs/include/**.h", "libs/flecs/src/**.h", "libs/flecs/src/**.c" }


project "main"
	kind "ConsoleApp"
	location "project/"
	language "C++"
	cppdialect  "C++20"
	intrinsics "On"
	filter "configurations:Release"
		optimize "Speed"
	filter {}
	targetdir "bin/%{cfg.buildcfg}"

  dependson { "raylib", "box2d", "flecs"}

	vpaths
	{
		["_Resources/*"] = { "project/assets/**" },
		["Engine Files/*"] = { "project/src/engine/**.cpp", "project/src/engine/**.h" },
		["_Resources/Windows"] = { "project/resource/mainIcon.ico", "project/resource/resource.h", "project/resource/main.rc" },
	}
	links {"raylib","box2d", "bixxy-engine", "flecs"}
	includedirs {
		"project/src",  "libs/bixxy-engine",
		"libs/raylib/src",
		"libs/flecs",
		"libs/raylib/src/external/glfw/include",
		"libs/box2d/include"
		 }

	filter "action:vs*"
		defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
		dependson {"raylib", "box2d", "flecs"}
		links {"raylib.lib", "flecs.lib"}
        characterset ("MBCS")

	filter "system:windows"
		files {
			"project/src/**.cpp", "project/src/**.h",  --main files
			"project/resource/mainIcon.ico", "project/resource/resource.h", "project/resource/main.rc", --assets
			"project/assets/shaders/*.glsl" --shaders
		}
		defines{"_WIN32"}
		links {"winmm", "kernel32", "opengl32", "kernel32", "gdi32"}
		libdirs {"bin/%{cfg.buildcfg}"}

  filter {"system:windows", "action:gmake*"}
    links{"Ws2_32"}

	filter "system:linux"
		files {
			"project/src/**.cpp", "project/src/**.h",  --main files
			"project/assets/shaders/*.glsl" --shaders
		}

		postbuildcommands {
		  "{COPY} assets %{cfg.targetdir}"
		}

		links {"pthread", "GL", "m", "dl", "rt", "X11", "atomic"}

