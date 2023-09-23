--premake5.lua - Build File

project "MathLib"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir "%{wks.location}/%{prj.name}/Builds/%{cfg.buildcfg}/%{cfg.platform}"
	objdir "%{wks.location}/%{prj.name}/Builds-Int/%{cfg.buildcfg}/%{cfg.platform}"

	pchsource "%{prj.location}/Source/MathPCH.cpp"

	filter { "system:Windows" }
		pchheader "MathPCH.h"
	filter { }
	
	filter { "system:MacOSx" }
		pchheader "%{prj.location}/Source/MathPCH.h"
	filter { }

	files
	{
		"Source/**.h",
		"Source/**.cpp",
		"Source/**.hpp"
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"