--premake5.lua

project "UnitTests"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

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

	--================================= END ENGINE DEPENDENCIES =============================--
