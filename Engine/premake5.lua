-- premake5.lua - Build File for Engine

project "Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	pchheader "EnginePCH.h"
	pchsource "%{prj.location}/Source/EnginePCH.cpp"

	targetdir "%{wks.location}/%{prj.name}/Builds/%{cfg.buildcfg}/%{cfg.platform}/"
	objdir "%{wks.location}/%{prj.name}/Builds-Int/%{cfg.buildcfg}/%{cfg.platform}/"

	files
	{
		"%{prj.location}/Source/",
		"%{prj.location}/Source/**.h",
		"%{prj.location}/Source/**.cpp",
		"%{prj.location}/Source/**.hpp",

		"%{IncludeDirectories.entt}**.h",
		"%{IncludeDirectories.entt}**.cpp",
		"%{IncludeDirectories.entt}**.hpp",

		"%{IncludeDirectories.rapidjson}**.h",

		"%{IncludeDirectories.ImGuizmo}GraphEditor.cpp",
		"%{IncludeDirectories.ImGuizmo}GraphEditor.h",
		"%{IncludeDirectories.ImGuizmo}ImCurveEdit.cpp",
		"%{IncludeDirectories.ImGuizmo}ImCurveEdit.h",
		"%{IncludeDirectories.ImGuizmo}ImGradient.cpp",
		"%{IncludeDirectories.ImGuizmo}ImGradient.h",
		"%{IncludeDirectories.ImGuizmo}ImGuizmo.cpp",
		"%{IncludeDirectories.ImGuizmo}ImGuizmo.h",
		"%{IncludeDirectories.ImGuizmo}ImSequencer.cpp",
		"%{IncludeDirectories.ImGuizmo}ImSequencer.h",
		"%{IncludeDirectories.ImGuizmo}ImZoomSlider.h"
	}

	filter { "system:Windows" }
		flags { "NoPCH" }
		files
		{
			"%{IncludeDirectories.ImGui}backends/imgui_impl_glfw.h",
			"%{IncludeDirectories.ImGui}backends/imgui_impl_glfw.cpp",
			"%{IncludeDirectories.ImGui}backends/imgui_impl_dx11.h",
			"%{IncludeDirectories.ImGui}backends/imgui_impl_dx11.cpp",
			"%{IncludeDirectories.ImGui}backends/imgui_impl_win32.h",
			"%{IncludeDirectories.ImGui}backends/imgui_impl_win32.cpp",
		}

	includedirs
	{
		"%{prj.location}/Source/",
		"%{prj.location}/Source/**",

		"%{wks.location}/MathLib/",

		"%{IncludeDirectories.entt}",
		"%{IncludeDirectories.glfw}",
		"%{IncludeDirectories.spdlog}",
		"%{IncludeDirectories.rapidjson}",
		"%{IncludeDirectories.ImGui}",
		"%{IncludeDirectories.ImGuizmo}"
	}

	-- ImGuizmo macros.
	defines { "USE_IMGUI_API "}

	-- Linking Libraries.
	links
	{
		"MathLib",
		"%{LibraryNames.glfw}",
		"%{LibraryNames.ImGui}"
	}

	-- Linking Corresponding Library Directories.
	libdirs
	{
		"%{LibraryDirectories.MathLib}Builds/%{cfg.buildcfg}/%{cfg.platform}/",
		"%{LibraryDirectories.glfw}Builds/%{cfg.buildcfg}/%{cfg.platform}/",
		"%{LibraryDirectories.ImGui}Builds/%{cfg.buildcfg}/%{cfg.platform}/"
	}

	filter "configurations:Debug"
		defines { "DEBUG", "_DEBUG" }
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		defines { "RELEASE", "NDEBUG" }
		optimize "On"
		runtime "Release"

	filter { "system:Windows" }

		includedirs
		{
			"%{IncludeDirectories.DirectXTK}",
			"C:/Program Files/Autodesk/FBX/FBX SDK/2020.2/include/"
		}

		links
		{
			"%{LibraryNames.DirectXTK}",
			"libfbxsdk.lib"
		}

		defines
		{
			"PLATFORM_WINDOWS",
			"FBXSDK_SHARED"
		}

		postbuildcommands 
		{
			"xcopy /Y /E /I \"%{prj.location}Shaders\" \"%{startprojectpath}/Shaders\""
		}

		-- Pre Build command so that the MathLib gets built before Engine.

	filter { "architecture:x86", "system:Windows" }
		libdirs
		{
			"%{LibraryDirectories.DirectXTK}Bin/Desktop_2019/Win32/%{cfg.buildcfg}/",
			"C:/Program Files/Autodesk/FBX/FBX SDK/2020.2/lib/vs2019/x86/%{cfg.buildcfg}/"
		}
		-- Pre Build Commands so that glfw, imgui & DirectXTK gets built before the Engine lib.
		prebuildcommands
		{
			"msbuild \"..\\Engine\\Libraries\\DirectXTK\\DirectXTK_Desktop_2019.vcxproj\" /p:Configuration=%{cfg.buildcfg} /p:platform=win32",
			"msbuild \"..\\Engine\\Libraries\\glfw\\glfw.vcxproj\" /p:Configuration=\"%{cfg.buildcfg}\" /p:platform=win32",
			"msbuild \"..\\Engine\\Libraries\\ImGui\\ImGui.vcxproj\" /p:Configuration=\"%{cfg.buildcfg}\" /p:platform=win32"
		}

		postbuildcommands
		{
			"mkdir \"%{startprojectpath}\\Builds\\%{cfg.buildcfg}\\Win32\"",
			"copy /Y \"C:\\Program Files\\Autodesk\\FBX\\FBX SDK\\2020.2\\lib\\vs2019\\x86\\%{cfg.buildcfg}\\libfbxsdk.dll\" \"%{startprojectpath}\\Builds\\%{cfg.buildcfg}\\Win32\\libfbxsdk.dll\""
		}

		defines
		{
			"PLATFORM_WINDOWS_X86"
		}
	filter { "architecture:x86_64", "system:Windows"}
		libdirs
		{
			"%{LibraryDirectories.DirectXTK}Bin/Desktop_2019/x64/%{cfg.buildcfg}/",
			"C:/Program Files/Autodesk/FBX/FBX SDK/2020.2/lib/vs2019/x64/%{cfg.buildcfg}/"
		}
		-- Pre Build Commands so that glfw, imgui & DirectXTK gets built before the Engine lib.
		prebuildcommands
		{
			"msbuild \"..\\Engine\\Libraries\\DirectXTK\\DirectXTK_Desktop_2019.vcxproj\" /p:Configuration=%{cfg.buildcfg} /p:platform=x64",
			"msbuild \"..\\Engine\\Libraries\\glfw\\glfw.vcxproj\" /p:Configuration=\"%{cfg.buildcfg} Win64\" /p:platform=x64",
			"msbuild \"..\\Engine\\Libraries\\ImGui\\ImGui.vcxproj\" /p:Configuration=\"%{cfg.buildcfg} Win64\" /p:platform=x64"
		}
		postbuildcommands
		{
			"mkdir \"%{startprojectpath}\\Builds\\%{cfg.buildcfg}\\Win64\"",
			"copy /Y \"C:\\Program Files\\Autodesk\\FBX\\FBX SDK\\2020.2\\lib\\vs2019\\x64\\%{cfg.buildcfg}\\libfbxsdk.dll\" \"%{startprojectpath}\\Builds\\%{cfg.buildcfg}\\Win64\\libfbxsdk.dll\""
		}

		defines
		{
			"PLATFORM_WINDOWS_X64"
		}