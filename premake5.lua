workspace "OpenGL-Project-3D"
	configurations { "Debug", "Release" }

include "Build/premake/actions.lua"
include "Vendor/glfw"
include "Vendor/glad"

project "OpenGL-Project-3D"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir "Bin/%{cfg.buildcfg}"
	objdir "Obj/%{cfg.buildcfg}"

	files { "Source/**.cpp", "Source/**.h" }

	includedirs
	{
		"Vendor/glfw/include",
		"Vendor/glad/include"
	}

	links
	{
		"glfw",
		"Gdi32",
		"Imm32",
		"glad",
	}

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "on"
		runtime "Debug"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "on"
		runtime "Release"
		