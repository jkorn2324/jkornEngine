--premake5.lua

project "Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	pchheader "EditorPCH.h"
	pchsource "%{prj.location}/Source/EditorPCH.cpp"
	
	targetdir "%{wks.location}/%{prj.name}/Builds/%{cfg.buildcfg}/%{cfg.platform}/"
	objdir "%{wks.location}/%{prj.name}/Builds-Int/%{cfg.buildcfg}/%{cfg.platform}/"

	files
	{
		"%{prj.location}/Source/",
		"%{prj.location}/Source/**.h",
		"%{prj.location}/Source/**.cpp",
		"%{prj.location}/Source/**.hpp",
	}

	includedirs
	{
		"%{prj.location}/Source/",
		"%{prj.location}/Source/**",

		"%{wks.location}/Engine/Source/",
		"%{wks.location}/Engine/Source/**",

		"%{wks.location}/MathLib/",

		"%{IncludeDirectories.entt}",
		"%{IncludeDirectories.ImGui}",
		"%{IncludeDirectories.spdlog}"
	}

	links
	{
		"%{LibraryNames.MathLib}",
		"Engine.lib"
	}

	libdirs
	{
		"../MathLib/Builds/%{cfg.buildcfg}/%{cfg.platform}/",
		"../Engine/Builds/%{cfg.buildcfg}/%{cfg.platform}/",
		"%{prj.location}/Builds/%{cfg.buildcfg}/%{cfg.platform}/"
	}

	filter "system:Windows"
		links { "d3d11.lib" }

	filter "configurations:Debug"
		defines { "DEBUG", "_DEBUG" }
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		defines { "RELEASE", "NDEBUG" }
		optimize "On"
		runtime "Release"
	
	filter "files:Source/EditorApplication.cpp"
		flags "NoPCH"

