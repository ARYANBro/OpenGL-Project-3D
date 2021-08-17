project "glad"
	kind "StaticLib"
	language "C"
	staticruntime "on"

	targetdir("%{prj.location}/Bin/%{cfg.buildcfg}/%{prj.name}")
	objdir("%{prj.location}/Obj/%{cfg.buildcfg}/%{prj.name}")

	files
	{
		"src/glad.c",
	}

	includedirs
	{
		"include"
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
