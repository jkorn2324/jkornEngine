--premake5.lua - Build File

include "Dependencies.lua"

workspace "jkornEngine"
	
	startupprojectname = "Editor"
	startproject "%{startupprojectname}"
	startprojectpath = "%{wks.location}%{startupprojectname}"

	configurations 
	{ 
		"Debug", 
		"Release" 
	}

	platforms
	{
		"Win64",
		"Win32"
	}

	filter { "platforms:x64" }
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

-- Create projects for static libraries.

group "Dependencies"
	include "Engine/Libraries/glfw.lua"
	include "Engine/Libraries/ImGui.lua"
group ""

include "MathLib"
include "Engine"
include "Editor"
include "GlfwSandboxProject"
include "UnitTests"
