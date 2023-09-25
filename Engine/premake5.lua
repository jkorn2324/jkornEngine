-- premake5.lua - Build File for Engine

project "Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir "%{wks.location}/%{prj.name}/Builds/%{cfg.buildcfg}/%{cfg.platform}/"
	objdir "%{wks.location}/%{prj.name}/Builds-Int/%{cfg.buildcfg}/%{cfg.platform}/"

	pchsource "%{prj.location}/Source/EnginePCH.cpp"

	filter { "system:Windows" }
		pchheader "EnginePCH.h"
	filter { }
	
	filter { "system:MacOSx" }
		pchheader "%{prj.location}/Source/EnginePCH.h"
	filter { }


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
	}

	postbuildcommands
	{
		"mkdir \"%{cfg.buildtarget.directory}\"",
		-- TODO: Remove this functionality as it only is hooked to the Editor Project
		"xcopy /Y /E /I \"%{prj.location}Shaders\" \"%{startprojectpath}/Shaders\""
	}

	-- Configuration Defines (Debug)
	filter "configurations:Debug"
		defines { "DEBUG", "_DEBUG" }
		symbols "On"
		runtime "Debug"
	filter { }

	-- Configuration Defines (Release)
	filter "configurations:Release"
		defines { "RELEASE", "NDEBUG" }
		optimize "On"
		runtime "Release"
	filter { }

	-- 	Additional Platform Defines (Win32)
	filter { "platforms:Win32" }
		defines
		{
			"PLATFORM_WINDOWS_X86"
		}
	-- Additional Platform Defines (Win64)
	filter { "platforms:Win64"}
		defines
		{
			"PLATFORM_WINDOWS_X64"
		}
	-- Additional Platform Defines (MacOS)
	filter { "platforms:MacOSx" }
		defines
		{
			"PLATFORM_MACOSX_X64"
		}
	filter { }


	--================================= BEGIN MATHLIB DEPENDENCY ===========================--

	includedirs
	{
		"%{IncludeDirectories.MathLib}"
	}

	links
	{
		"%{LibraryNames.MathLib}"
	}

	libdirs
	{
		"%{BuildDirectories.MathLib}%{cfg.buildcfg}/%{cfg.platform}/"
	}

	--================================== END MATHLIB DEPENDENCY ============================--

	--================================== BEGIN ENTT DEPENDENCY =============================--

	files
	{
		"%{IncludeDirectories.entt}**.h",
		"%{IncludeDirectories.entt}**.cpp",
		"%{IncludeDirectories.entt}**.hpp"
	}

	includedirs
	{
		"%{IncludeDirectories.entt}"
	}

	--================================== END ENTT DEPENDENCY ===============================--

	--================================== BEGIN RAPIDJSON DEPENDENCY ========================--

	files
	{
		"%{IncludeDirectories.rapidjson}**.h"
	}

	includedirs
	{
		"%{IncludeDirectories.rapidjson}"
	}

	--================================== END RAPIDJSON DEPENDENCY ==========================--

	--================================== BEGIN GLFW DEPENDENCY =============================--

	includedirs
	{
		"%{IncludeDirectories.glfw}"
	}

	-- Linking Libraries.
	links
	{
		"%{LibraryNames.glfw}"
	}

	-- Linking Corresponding Library Directories.
	libdirs
	{
		"%{BuildDirectories.glfw}%{cfg.buildcfg}/%{cfg.platform}/"
	}
		
	-- Pre Build Command that gets added for when the architecture is a x86 system and its on windows
	filter { "platforms:Win32" }
		-- Pre Build Commands so that glfw gets built before the Engine lib.
		prebuildcommands
		{
			"msbuild \"%{ProjectDirectories.glfw}glfw.vcxproj\" /p:Configuration=\"%{cfg.buildcfg}\" /p:platform=win32",
		}
	-- Pre Build Command that gets added for when the architecture is a x64 system and its on windows
	filter { "platforms:Win64"}
		-- Pre Build Commands so that glfw gets built before the Engine lib.
		prebuildcommands
		{
			"msbuild \"%{ProjectDirectories.glfw}glfw.vcxproj\" /p:Configuration=\"%{cfg.buildcfg} Win64\" /p:platform=x64",
		}
	filter { }

	--================================== END GLFW DEPENDENCY ===============================--

	--================================== BEGIN IMGUI DEPENDENCY ============================--

	-- TODO: Only do this if our Windowing Api System is glfw
	files
	{
		-- GLFW IMGUI FILES
		"%{IncludeDirectories.ImGui}backends/imgui_impl_glfw.h",
		"%{IncludeDirectories.ImGui}backends/imgui_impl_glfw.cpp"
	}
	
	includedirs
	{
		"%{IncludeDirectories.ImGui}"
	}

	links
	{
		"%{LibraryNames.ImGui}"
	}

	libdirs
	{
		"%{BuildDirectories.ImGui}%{cfg.buildcfg}/%{cfg.platform}/"
	}

	-- Ensures that we only add the imgui win32 files if our system is windows.
	filter { "system:Windows" }
		files
		{
			"%{IncludeDirectories.ImGui}backends/imgui_impl_win32.h",
			"%{IncludeDirectories.ImGui}backends/imgui_impl_win32.cpp",
		}
	-- Ensures that we only add imgui directx files if our system & graphics api are directx11 & windows
	filter { "system:Windows", "options:graphicsapi=directx11" }
		files
		{
			"%{IncludeDirectories.ImGui}backends/imgui_impl_dx11.h",
			"%{IncludeDirectories.ImGui}backends/imgui_impl_dx11.cpp"
		}
	filter { "platforms:Win32" }
		-- Pre Build Commands so that ImGui gets built before the Engine lib (Win32)
		prebuildcommands
		{
			"msbuild \"%{ProjectDirectories.ImGui}ImGui.vcxproj\" /p:Configuration=\"%{cfg.buildcfg}\" /p:platform=win32"
		}
	filter { "platforms:Win64" }
		-- Pre Build Commands so that ImGui gets built before the Engine lib (Win64)
		prebuildcommands
		{
			"msbuild \"%{ProjectDirectories.ImGui}ImGui.vcxproj\" /p:Configuration=\"%{cfg.buildcfg} Win64\" /p:platform=x64"
		}
	filter { }

	--================================ END IMGUI DEPENDENCY ========================================--

	--================================ BEGIN IMGUIZMO DEPENDENCY ===================================--

	files
	{
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

	includedirs
	{
		"%{IncludeDirectories.ImGuizmo}"
	}

	-- Macro for using ImGuizmo w/ ImGui
	defines { "USE_IMGUI_API "}


	--================================= END IMGUIZMO DEPENDENCY ====================================--

	--================================= BEGIN SPDLOG DEPENDENCY ====================================--

	includedirs
	{
		"%{IncludeDirectories.spdlog}"
	}

	--================================= END SPDLOG DEPENDENCY ======================================--


	--================================= BEGIN FBX DEPENDENCY ======================================--

	filter { "system:Windows" }
		includedirs
		{
			"%{IncludeDirectories.fbxsdk_windows}"
		}
		links
		{
			os.findlib("%{LibraryNames.fbxsdk_windows}")
		}
		defines
		{
			"FBXSDK_SHARED"
		}
	
	-- Define Post Build Actions (Depending on Platform & Action)
	filter { "platforms:Win32", "action:vs2022" }
		postbuildcommands
		{
			"copy /Y \"%{BuildDirectories.fbxsdk_windows}vs2022/x86/%{cfg.buildcfg}\\libfbxsdk.dll\" \"%{cfg.buildtarget.directory}libfbxsdk.dll\""
		}
		libdirs
		{
			"%{BuildDirectories.fbxsdk_windows}vs2022/x86/%{cfg.buildcfg}/"
		}
	filter { "platforms:Win32", "action:vs2019" }
		postbuildcommands
		{
			"copy /Y \"%{BuildDirectories.fbxsdk_windows}vs2019/x86/%{cfg.buildcfg}\\libfbxsdk.dll\" \"%{cfg.buildtarget.directory}libfbxsdk.dll\""
		}
		libdirs
		{
			"%{BuildDirectories.fbxsdk_windows}vs2019/x86/%{cfg.buildcfg}/"
		}
	filter { "platforms:Win64", "action:vs2022" }
		postbuildcommands
		{
			"copy /Y /V \"%{BuildDirectories.fbxsdk_windows}vs2022/x64/%{cfg.buildcfg}\\libfbxsdk.dll\" \"%{cfg.buildtarget.directory}libfbxsdk.dll\""
		}
		libdirs
		{
			"%{BuildDirectories.fbxsdk_windows}vs2022/x64/%{cfg.buildcfg}/"
		}
	filter { "platforms:Win64", "action:vs2019" }
		postbuildcommands
		{
			"copy /Y \"%{BuildDirectories.fbxsdk_windows}vs2019/x64/%{cfg.buildcfg}\\libfbxsdk.dll\" \"%{cfg.buildtarget.directory}libfbxsdk.dll\""
		}
		libdirs
		{
			"%{BuildDirectories.fbxsdk_windows}vs2019/x64/%{cfg.buildcfg}/"
		}
	filter { }

	--================================= END FBX DEPENDENCY ======================================--

	--================================= BEGIN DIRECTX11 + DIRECTXTK DEPENDENCY ==================--
	

	filter { "system:Windows", "options:graphicsapi=directx11" }
		includedirs
		{
			"%{IncludeDirectories.DirectXTK}",
		}
		links
		{
			"%{LibraryNames.d3d11}",
			"%{LibraryNames.DirectXTK}",
		}
	-- Platform is Win32 & action is vs2022
	filter { "platforms:Win32", "options:graphicsapi=directx11", "action:vs2022" }
		libdirs
		{
			"%{BuildDirectories.DirectXTK}Bin/Desktop_2022/Win32/%{cfg.buildcfg}/"
		}
		prebuildcommands
		{
			"msbuild \"%{ProjectDirectories.DirectXTK}DirectXTK_Desktop_2022.vcxproj\" /p:Configuration=%{cfg.buildcfg} /p:platform=win32",
		}
	-- Platform is Win64 & action is vs2022
	filter { "platforms:Win64", "options:graphicsapi=directx11", "action:vs2022" }
		libdirs
		{
			"%{BuildDirectories.DirectXTK}Bin/Desktop_2022/x64/%{cfg.buildcfg}/"
		}
		prebuildcommands
		{
			"msbuild \"%{ProjectDirectories.DirectXTK}DirectXTK_Desktop_2022.vcxproj\" /p:Configuration=%{cfg.buildcfg} /p:platform=x64",
		}
	-- Platform is Win32 & action is vs2019
	filter { "platforms:Win32", "options:graphicsapi=directx11", "action:vs2019" }
		libdirs
		{
			"%{BuildDirectories.DirectXTK}Bin/Desktop_2019/Win32/%{cfg.buildcfg}/"
		}
		prebuildcommands
		{
			"msbuild \"%{ProjectDirectories.DirectXTK}DirectXTK_Desktop_2019.vcxproj\" /p:Configuration=%{cfg.buildcfg} /p:platform=win32",
		}
	-- Platform is Win64 & action is vs2019
	filter { "platforms:Win64", "options:graphicsapi=directx11", "action:vs2019" }
		libdirs
		{
			"%{BuildDirectories.DirectXTK}Bin/Desktop_2019/x64/%{cfg.buildcfg}/"
		}
		prebuildcommands
		{
			"msbuild \"%{ProjectDirectories.DirectXTK}DirectXTK_Desktop_2019.vcxproj\" /p:Configuration=%{cfg.buildcfg} /p:platform=x64",
		}
	filter { }

	--================================= END DIRECTX11 + DIRECTXTK DEPENDENCY ====================--


	-- Ensures that all of the files outside of the source file don't have precompiled headers associated with them.
	filter { "files:not %{prj.location}/Source/**.h", "files:not {%prj.location}/Source/**.cpp", "files:not {%prj.location}/Source/**.hpp" }
		flags { "NoPCH" }
	filter { }



