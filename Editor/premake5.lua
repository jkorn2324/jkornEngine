--premake5.lua

project "Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	-- Removes the platforms given the current platform being generated.
	filter { "system:Windows" }
		removeplatforms  { "MacOS" }
	filter { "system:MacOSx" }
		removeplatforms { "Win64", "Win32" }
	filter { }

	targetdir "%{wks.location}/%{prj.name}/Builds/%{cfg.buildcfg}/%{cfg.platform}/"
	objdir "%{wks.location}/%{prj.name}/Builds-Int/%{cfg.buildcfg}/%{cfg.platform}/"

	pchsource "%{prj.location}/Source/EditorPCH.cpp"
	
	filter { "system:Windows" }
		pchheader "EditorPCH.h"
	filter { "system:MacOSx" }
		pchheader "%{prj.location}/Source/EditorPCH.h"
	filter { }

	files
	{
		"%{prj.location}/Source/",
		"%{prj.location}/Source/**.h",
		"%{prj.location}/Source/**.cpp",
		"%{prj.location}/Source/**.hpp"
	}

	includedirs
	{
		"%{prj.location}/Source/",
		"%{prj.location}/Source/**"
	}

	libdirs
	{
		"%{prj.location}/Builds/%{cfg.buildcfg}/%{cfg.platform}/"
	}

	filter "configurations:Debug"
		defines { "DEBUG", "_DEBUG" }
		symbols "On"
		runtime "Debug"
	filter "configurations:Release"
		defines { "RELEASE", "NDEBUG" }
		optimize "On"
		runtime "Release"
	filter { }

	--================================= BEGIN ENGINE DEPENDENCIES ===========================--

	includedirs
	{
		"%{wks.location}/Engine/Source/",
		"%{wks.location}/Engine/Source/**",

		-- Engine Dependencies
		"%{IncludeDirectories.entt}",
		"%{IncludeDirectories.ImGui}",
		"%{IncludeDirectories.spdlog}",
		"%{IncludeDirectories.ImGuizmo}",
		"%{IncludeDirectories.rapidjson}",
		"%{IncludeDirectories.MathLib}"
	}

	links
	{
		"Engine"
	}

	libdirs
	{
		"%{wks.location}/Engine/Builds/%{cfg.buildcfg}/%{cfg.platform}/"
	}

	include_apple_frameworks()

	--================================= END ENGINE DEPENDENCIES =============================--
	
	filter "files:Source/EditorApplication.cpp"
		flags "NoPCH"
	filter { }
