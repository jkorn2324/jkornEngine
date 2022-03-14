-- imgui.lua - Build File

project "ImGui"
	kind "StaticLib"
	language "C++"
	staticruntime "off"
	cppdialect "C++17"

    location "%{wks.location}/Engine/Libraries/ImGui/"

	targetdir "%{wks.location}/Engine/Libraries/Builds/ImGui/Builds/%{cfg.buildcfg}/%{cfg.platform}/"
	objdir "%{wks.location}/Engine/Libraries/Builds/ImGui/Builds-Int/%{cfg.buildcfg}/%{cfg.platform}/"

	files
	{
		"%{prj.location}/*.h",
		"%{prj.location}/*.cpp"
	}

    includedirs
    {
        "%{prj.location}/*.h",
        "%{prj.location}/*.cpp",
    }

	filter "system:Windows"
		systemversion "latest"
		cppdialect "C++17"

	filter "system:Linux"
		pic "On"
		systemversion "latest"
		cppdialect "C++17"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"