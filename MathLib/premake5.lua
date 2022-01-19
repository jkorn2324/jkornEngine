--premake5.lua - Build File

project "MathLib"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"

	pchheader "MathPCH.h"
	pchsource "Source/MathPCH.cpp"

	targetdir "%{wks.location}/%{prj.name}/Builds/%{cfg.buildcfg}/"
	objdir "%{wks.location}/%{prj.name}/Builds-Int/%{cfg.buildcfg}/"

	files
	{
		"Source/**.h",
		"Source/**.cpp",
		"Source/**.hpp"
	}