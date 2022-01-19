--premake5.lua - Build File

include "Dependencies.lua"

workspace "jkornEngine"
	
	startproject "Editor"
	startprojectpath = "%{wks.location}/Editor/"

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
filter { "system:Windows"}
	include "Engine/Libraries/directxtk.lua"

include "MathLib"
include "Engine"
