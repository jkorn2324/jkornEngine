-- premake5.lua - Build File for Engine

project "Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"

	pchheader "EnginePCH.h"
	pchsource "%{prj.location}/Source/EnginePCH.cpp"

	targetdir "%{wks.location}/%{prj.name}/Builds/%{cfg.buildcfg}/"
	objdir "%{wks.location}/%{prj.name}/Builds-Int/%{cfg.buildcfg}/"

	files
	{
		"%{prj.location}/Source/**.h",
		"%{prj.location}/Source/**.cpp",
		"%{prj.location}/Source/**.hpp",

		"%{IncludeDirectories.entt}**.h",
		"%{IncludeDirectories.entt}**.cpp",
		"%{IncludeDirectories.entt}**.hpp",

		"%{IncludeDirectories.imgui}*.h",

		"%{IncludeDirectories.imgui}backends/imgui_impl_glfw.h",
		"%{IncludeDirectories.imgui}backends/imgui_impl_win32.h",
		"%{IncludeDirectories.imgui}backends/imgui_impl_dx11.h",

		"%{IncludeDirectories.rapidjson}**.h"
	}

	includedirs
	{
		"%{prj.location}/Source/",
		"%{prj.location}/Source/**",

		"%{wks.location}/MathLib/",

		"%{IncludeDirectories.entt}",
		"%{IncludeDirectories.glfw}",
		"%{IncludeDirectories.imgui}",
		"%{IncludeDirectories.spdlog}",
		"%{IncludeDirectories.rapidjson}"
	}

	-- Linking Libraries.
	links
	{
		"%{LibraryNames.MathLib}",
		"%{LibraryNames.glfw}",
	}
	-- Linking Corresponding Library Directories.
	libdirs
	{
		"%{LibraryDirectories.MathLib}",
		"%{LibraryDirectories.glfw}"
	}


	filter { "system:Windows" }
		includedirs
		{
			"%{IncludeDirectories.DirectXTK}"
		}

		links
		{
			"%{LibraryNames.DirectXTK}",
		}

		libdirs
		{
			"%{LibraryDirectories.DirectXTK}%{buildcfg}/"
		}

		defines
		{
			"PLATFORM_WINDOWS",
			"FBXSDK_SHARED"
		}

		postbuildcommands 
		{
			"xcopy \"%{prj.location}Shaders/*.*\" \"%{startprojectpath}Shaders/\" /Y /I /E"
		}

