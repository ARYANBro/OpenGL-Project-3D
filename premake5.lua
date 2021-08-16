workspace "OpenGL-Project-3D"
	configurations { "Debug", "Release" }

include "Build/premake/actions.lua"

project "OpenGL-Project-3D"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir "Bin/%{cfg.buildcfg}"
	objdir "Obj/%{cfg.buildcfg}"

	files { "Source/**.cpp", "Source/**.h" }

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "on"
		runtime "Debug"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "on"
		runtime "Release"
		