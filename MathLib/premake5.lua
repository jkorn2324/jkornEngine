--premake5.lua - Build File

project "MathLib"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	pchheader "%{prj.location}/Source/MathPCH.h"
	pchsource "%{prj.location}/Source/MathPCH.cpp"

	targetdir "%{wks.location}/%{prj.name}/Builds/%{cfg.buildcfg}/%{cfg.platform}"
	objdir "%{wks.location}/%{prj.name}/Builds-Int/%{cfg.buildcfg}/%{cfg.platform}"

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