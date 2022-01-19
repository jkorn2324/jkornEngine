-- directxtk.lua - Build File for DirectXTK

project "directxtk"
	location "%{wks.location}/Engine/Libraries/DirectXTK/"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"

	targetdir "%{prj.location}/Builds/%{cfg.buildcfg}/"
	objdir "%{prj.location}/Builds-Int/%{cfg.buildcfg}/"

	files
	{
		"%{prj.location}/Src/**.h",
		"%{prj.location}/Src/**.cpp",
		"%{prj.location}/Src/**.hpp",
		
		"%{prj.location}/Inc/**.h",
		"%{prj.location}/Inc/**.cpp",
		"%{prj.location}/Inc/**.hpp"
	}

	includedirs
	{
		"%{prj.location}/Inc/",
		"%{prj.location}/Src/"
	}