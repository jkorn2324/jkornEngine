--premake5.lua

project "Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	-- TODO: Target & Output Directories

	files 
	{
		"Source/**.h",
		"Source/**.cpp",
		"Source/**.hpp"
	}

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "RELEASE" }
		optimize "On"

	links
	{
		"MathLib",
		"Engine"
	}