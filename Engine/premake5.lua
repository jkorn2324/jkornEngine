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
	filter { "system:MacOSx" }
		pchheader "%{prj.location}/Source/EnginePCH.h"
	filter { }

	files
	{
		"%{prj.location}/Source/",
		"%{prj.location}/Source/**.h",
		"%{prj.location}/Source/**.cpp",
		"%{prj.location}/Source/**.hpp",
		"%{prj.location}/Source/**.mm"
	}

	-- Removes DirectX Related Files from workspace.
	filter { "options:not graphicsapi=directx11" }
		removefiles
		{
			"%{prj.location}/Source/Platform/DirectX11/**.h",
			"%{prj.location}/Source/Platform/DirectX11/**.cpp",
			"%{prj.location}/Source/Platform/DirectX11/**.hpp"
		}
	-- Removes the metal-cpp files from workspace
	filter { "options:not graphicsapi=metal-cpp" }
	filter { }

	includedirs
	{
		"%{prj.location}/Source/",
		"%{prj.location}/Source/**",
	}

	postbuildcommands
	{
		"mkdir -p \"%{cfg.buildtarget.directory}\"",
	}

	-- Only Copy Shaders like this if it is windows.

	if build_system == "windows" then

		postbuildcommands
		{
			-- TODO: Remove this functionality as it only is hooked to the Editor Project
			"xcopy /Y /E /I \"%{prj.location}Shaders\" \"%{startprojectpath}/Shaders\""
		}

	end
	if build_system == "macosx" then
	
		postbuildcommands
		{
			-- TODO: Remove this functionality as it only is hooked to the Editor Project
			"cp -R \"%{prj.location}/Shaders\" \"%{startprojectpath}/\""
		}

	end

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
	filter { "platforms:MacOS" }
		defines
		{
			"PLATFORM_MACOSX_X64"
		}
	filter { }

	include_apple_frameworks()

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

	filter { "action:vs*" }
		includedirs
		{
			"%{IncludeDirectories.entt}"
		}
	filter { "action:xcode*" }
		externalincludedirs
		{
			"%{IncludeDirectories.entt}"
		}
	filter { }

	--================================== END ENTT DEPENDENCY ===============================--

	--================================== BEGIN RAPIDJSON DEPENDENCY ========================--

	filter { "action:vs*" }
		includedirs
		{
			"%{IncludeDirectories.rapidjson}"
		}
	filter { "action:xcode4" }
		externalincludedirs
		{
			"%{IncludeDirectories.rapidjson}"
		}
	filter { }

	--================================== END RAPIDJSON DEPENDENCY ==========================--

	--================================== BEGIN GLFW DEPENDENCY =============================--

	filter { "action:xcode4" }
		externalincludedirs
		{
			"%{IncludeDirectories.glfw}"
		}
	filter { "action:vs*" }
		includedirs
		{
			"%{IncludeDirectories.glfw}"
		}
	filter { }

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

	--================================== END GLFW DEPENDENCY ===============================--

	--================================== BEGIN IMGUI DEPENDENCY ============================--

	-- TODO: Only do this if our Windowing Api System is glfw
	files
	{
		-- GLFW IMGUI FILES
		"%{IncludeDirectories.ImGui}backends/imgui_impl_glfw.h",
		"%{IncludeDirectories.ImGui}backends/imgui_impl_glfw.cpp"
	}

	-- Only include these files if we are in xcode.
	filter { "action:xcode4" }
		externalincludedirs
		{
			"%{IncludeDirectories.ImGui}"
		}
	filter { "action:vs*" }
		includedirs
		{
			"%{IncludeDirectories.ImGui}"
		}
	filter { }
	

	links
	{
		"%{LibraryNames.ImGui}"
	}

	libdirs
	{
		"%{BuildDirectories.ImGui}%{cfg.buildcfg}/%{cfg.platform}/"
	}

	-- Ensures that we only add the imgui osx files if our platform is windows
	filter { "platforms:MacOS" }
		files
		{
			"%{IncludeDirectories.ImGui}backends/imgui_impl_osx.h",
			"%{IncludeDirectories.ImGui}backends/imgui_impl_osx.mm"
		}
	-- Ensures that we only add the imgui metal files if our graphics api is metal.
	filter { "platforms:MacOS", "options:graphicsapi=metal" }
		files
		{
			"%{IncludeDirectories.ImGui}backends/imgui_impl_metal.h",
			"%{IncludeDirectories.ImGui}backends/imgui_impl_metal.mm"
		}
	-- Ensures that we only add the imgui win32 files if our platform is windows.
	filter { "platforms:Win32 or Win64" }
		files
		{
			"%{IncludeDirectories.ImGui}backends/imgui_impl_win32.h",
			"%{IncludeDirectories.ImGui}backends/imgui_impl_win32.cpp",
		}
	-- Ensures that we only add imgui directx files if our system & graphics api are directx11 & windows
	filter { "platforms:Win32 or Win64", "options:graphicsapi=directx11" }
		files
		{
			"%{IncludeDirectories.ImGui}backends/imgui_impl_dx11.h",
			"%{IncludeDirectories.ImGui}backends/imgui_impl_dx11.cpp"
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
	defines { "USE_IMGUI_API"}


	--================================= END IMGUIZMO DEPENDENCY ====================================--

	--================================= BEGIN SPDLOG DEPENDENCY ====================================--


	-- Only include these if the platform is vs2022
	filter { "action:vs*" }
		includedirs
		{
			"%{IncludeDirectories.spdlog}"
		}
	-- Only Include these if our current action is xcode4.
	filter { "action:xcode4" }
		externalincludedirs
		{
			"%{IncludeDirectories.spdlog}"
		}
	filter { }

	--================================= END SPDLOG DEPENDENCY ======================================--


	--================================= BEGIN FBX DEPENDENCY ======================================--

	filter { "platforms:Win32 or Win64" }
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
	-- This removes files associated with fbx if we don't have it
	filter { "platforms:not Win32 or Win64" }
		removefiles
		{
			"%{prj.location}/**/FBX*.cpp",
			"%{prj.location}/**/FBX*.h"
		}
	filter { }
	
	-- Define Post Build Actions (Depending on Platform & Action)
	filter { "platforms:Win32", "action:vs2022" }
		-- Ensures that the copy command only gets invoked for windows
		if build_system == "windows" then
			postbuildcommands
			{
				"copy /Y \"%{BuildDirectories.fbxsdk_windows}vs2022/x86/%{cfg.buildcfg}\\libfbxsdk.dll\" \"%{cfg.buildtarget.directory}libfbxsdk.dll\""
			}
		end
		-- TODO: FbxSDK

		libdirs
		{
			"%{BuildDirectories.fbxsdk_windows}vs2022/x86/%{cfg.buildcfg}/"
		}
	filter { "platforms:Win32", "action:vs2019" }

		-- Ensures that the copy command only gets invoked for windows
		if build_system == "windows" then
			postbuildcommands
			{
				"copy /Y \"%{BuildDirectories.fbxsdk_windows}vs2019/x86/%{cfg.buildcfg}\\libfbxsdk.dll\" \"%{cfg.buildtarget.directory}libfbxsdk.dll\""
			}
		end
		-- TODO: Sdk

		libdirs
		{
			"%{BuildDirectories.fbxsdk_windows}vs2019/x86/%{cfg.buildcfg}/"
		}
	filter { "platforms:Win64", "action:vs2022" }

		-- Ensures that the copy command only gets invoked for windows
		if build_system == "windows" then
			postbuildcommands
			{
				"copy /Y /V \"%{BuildDirectories.fbxsdk_windows}vs2022/x64/%{cfg.buildcfg}\\libfbxsdk.dll\" \"%{cfg.buildtarget.directory}libfbxsdk.dll\""
			}
		end

		libdirs
		{
			"%{BuildDirectories.fbxsdk_windows}vs2022/x64/%{cfg.buildcfg}/"
		}
	filter { "platforms:Win64", "action:vs2019" }

		-- Ensures that the copy command only gets invoked for windows
		if build_system == "windows" then
			postbuildcommands
			{
				"copy /Y \"%{BuildDirectories.fbxsdk_windows}vs2019/x64/%{cfg.buildcfg}\\libfbxsdk.dll\" \"%{cfg.buildtarget.directory}libfbxsdk.dll\""
			}
		end

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

		-- ensures that the msbuild commands only get run for windows.
		if build_system == "windows" then
			prebuildcommands
			{
				"msbuild \"%{ProjectDirectories.DirectXTK}DirectXTK_Desktop_2022.vcxproj\" /p:Configuration=%{cfg.buildcfg} /p:platform=win32",
			}
		end

	-- Platform is Win64 & action is vs2022
	filter { "platforms:Win64", "options:graphicsapi=directx11", "action:vs2022" }
		libdirs
		{
			"%{BuildDirectories.DirectXTK}Bin/Desktop_2022/x64/%{cfg.buildcfg}/"
		}

		-- ensures that the msbuild commands only get run for windows.
		if build_system == "windows" then
			prebuildcommands
			{
				"msbuild \"%{ProjectDirectories.DirectXTK}DirectXTK_Desktop_2022.vcxproj\" /p:Configuration=%{cfg.buildcfg} /p:platform=x64",
			}
		end

	-- Platform is Win32 & action is vs2019
	filter { "platforms:Win32", "options:graphicsapi=directx11", "action:vs2019" }
		libdirs
		{
			"%{BuildDirectories.DirectXTK}Bin/Desktop_2019/Win32/%{cfg.buildcfg}/"
		}

		-- ensures that the msbuild commands only get run for windows.
		if build_system == "windows" then
			prebuildcommands
			{
				"msbuild \"%{ProjectDirectories.DirectXTK}DirectXTK_Desktop_2019.vcxproj\" /p:Configuration=%{cfg.buildcfg} /p:platform=win32",
			}
		end

	-- Platform is Win64 & action is vs2019
	filter { "platforms:Win64", "options:graphicsapi=directx11", "action:vs2019" }
		libdirs
		{
			"%{BuildDirectories.DirectXTK}Bin/Desktop_2019/x64/%{cfg.buildcfg}/"
		}

		-- ensures that the msbuild commands only get run for windows.
		if build_system == "windows" then
			prebuildcommands
			{
				"msbuild \"%{ProjectDirectories.DirectXTK}DirectXTK_Desktop_2019.vcxproj\" /p:Configuration=%{cfg.buildcfg} /p:platform=x64",
			}
		end
	filter { }

	--================================= END DIRECTX11 + DIRECTXTK DEPENDENCY ====================--

	--================================= BEGIN METAL + QUARTZCORE DEPENDENCY ====================--

	include_metal_frameworks()

	--================================= END METAL + QUARTZCORE DEPENDENCY ====================--

	-- Ensures that all of the files outside of the source file don't have precompiled headers associated with them.
	filter { "files:not %{prj.location}/Source/**.h", "files:not %{prj.location}/Source/**.cpp", "files:not %{prj.location}/Source/**.hpp" }
		flags { "NoPCH" }
	filter { }

	-- Ensures that the files included in the source project are compiled as objective c++
	filter { "action:xcode*", "platforms:MacOS", "files:Source/**.cpp" }
		compileas "Objective-C++"
	filter { }




