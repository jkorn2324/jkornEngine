--premake5.lua - Build File

include "Dependencies.lua"

build_system = os.target()

if build_system == "windows" then
	-- Defines the graphics api options
	newoption {
		trigger = "graphicsapi",
		value = "API",
		description = "Choose a particular graphics API",
		allowed = {
			{ "directx11", "DirectX11 (Windows Only)" },
			{ "metal", "Metal (Apple Only)" },
			{ "none", "No Graphics" }
		},
		default = "directx11"
	}

end

if build_system == "macosx" then

	-- Defines the graphics api options
	newoption {
		trigger = "graphicsapi",
		value = "API",
		description = "Choose a particular graphics API",
		allowed = {
			{ "directx11", "DirectX11 (Windows Only)" },
			{ "metal", "Metal (Apple Only)" },
			{ "none", "No Graphics" }
		},
		default = "metal"
	}

end

workspace "jkornEngine"
	
	-- TODO: Remove this functionality
	startupprojectname = "Editor"
	startproject "%{startupprojectname}"
	startprojectpath = "%{wks.location}%{startupprojectname}"

	configurations 
	{ 
		-- The debug configuration
		"Debug",
		-- The release configuration
		"Release"
	}

	platforms
	{
		"Win64",
		"Win32",
		"MacOS"
	}

	filter { "platforms:Win64" }
		system "Windows"
		architecture "x86_64"

		defines 
		{
			"PLATFORM_WINDOWS",
			"PLATFORM_WINDOWS_X64"
		}

	filter { "platforms:Win32"}
		system "Windows"
		architecture "x86"

		defines
		{
			"PLATFORM_WINDOWS",
			"PLATFORM_WINDOWS_X86"
		}

	filter { "platforms:MacOS" }
		system "MacOSx"
		architecture "x86_64"

		defines
		{
			"PLATFORM_OSX",
			"PLATFORM_OSX_X64",
			"PLATFORM_MACOS"
		}

	filter { "configurations:Debug" }
		defines 
		{
			"DEBUG",
			"_DEBUG"
		}
	filter { "configurations:Release" }
		defines
		{
			"RELEASE",
			"NDEBUG"
		}
	filter { }

	-- DEFINE GRAPHICS API MACROS --
	filter { "system:Windows", "options:graphicsapi=directx11" }
		defines
		{
			"GRAPHICS_API_DIRECTX11"
		}
	filter { "system:MacOSx", "options:graphicsapi=metal" }
		defines
		{
			"GRAPHICS_API_METAL"
		}
	filter { }

-- Create projects for static libraries.

group "Dependencies"
	include "Engine/Libraries/glfw.lua"
	include "Engine/Libraries/ImGui.lua"
group ""

group "Engine/Modules"
	include "MathLib"
group ""

group "Engine"
	include "Engine"
	include "Editor"
group ""

group "Misc"
	include "GlfwSandboxProject"
	include "UnitTests"
group ""