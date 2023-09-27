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

	filter { "action:xcode4" }
		xcodebuildsettings 
		{ 
			["ALWAYS_SEARCH_USER_PATHS"] = "YES" 
		}
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
	}

	-- Only Copy Shaders like this if it is windows.

	build_system = os.target()
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
			"cp -R \"%{prj.location}Shaders\" \"%{startprojectpath}/Shaders\""
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


	--================================= BEGIN MATHLIB DEPENDENCY ===========================--

	-- "Only include files to compile if its xcode4"
	filter { "action:xcode4" }
		files
		{
			"%{IncludeDirectories.MathLib}**.cpp",
			"%{IncludeDirectories.MathLib}**.h",
			"%{IncludeDirectories.MathLib}**.hpp"
		}
	filter { }

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

	filter { "action:xcode4" }
		files
		{
			"%{IncludeDirectories.glfw}**.hpp",
			"%{IncludeDirectories.glfw}**.h",
			"%{IncludeDirectories.glfw}**.cpp"
		}
	filter { }

	includedirs
	{
		"%{IncludeDirectories.glfw}"
	}

	filter { "action:xcode4" }
		externalincludedirs
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

	-- "Ensures that the prebuild commands only gets added if our system is windows"
	if build_system == "windows" then

	-- Pre Build Command that gets added for when the architecture is a x86 system and its on windows
	filter { "platforms:Win32", "action:vs*" }
		-- Pre Build Commands so that glfw gets built before the Engine lib.
		prebuildcommands
		{
			"msbuild \"%{ProjectDirectories.glfw}glfw.vcxproj\" /p:Configuration=\"%{cfg.buildcfg}\" /p:platform=win32",
		}
	-- Pre Build Command that gets added for when the architecture is a x64 system and its on windows
	filter { "platforms:Win64", "action:vs*" }
		-- Pre Build Commands so that glfw gets built before the Engine lib.
		prebuildcommands
		{
			"msbuild \"%{ProjectDirectories.glfw}glfw.vcxproj\" /p:Configuration=\"%{cfg.buildcfg} Win64\" /p:platform=x64",
		}
	filter { }

	end

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
		files
		{
			"%{IncludeDirectories.ImGui}*.cpp",
			"%{IncludeDirectories.ImGui}*.h",
			"%{IncludeDirectories.ImGui}*.hpp"
		}
	filter { }
	
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

	-- Ensures that we only add the imgui osx files if our platform is windows
	filter { "platforms:MacOS" }
		files
		{
			"%{IncludeDirectories.ImGui}backends/imgui_impl_osx.h",
			"%{IncludeDirectories.ImGui}backends/imgui_impl_osx.mm"
		}
	-- Ensures that we only add the imgui metal files if our graphics api is metal.
	filter { "system:MacOSx", "options:graphicsapi=metal" }
		files
		{
			"%{IncludeDirectories.ImGui}backends/imgui_impl_metal.h",
			"%{IncludeDirectories.ImGui}backends/imgui_impl_metal.mm"
		}
	-- Ensures that we only add the imgui win32 files if our platform is windows.
	filter { "platforms:Win64", "platforms:Win32" }
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
	filter { }

	-- Ensures that this prebuild commands only occurs for windows
	if build_system == "windows" then
	
	filter { "platforms:Win32", "action:vs*" }
		-- Pre Build Commands so that ImGui gets built before the Engine lib (Win32)
		prebuildcommands
		{
			"msbuild \"%{ProjectDirectories.ImGui}ImGui.vcxproj\" /p:Configuration=\"%{cfg.buildcfg}\" /p:platform=win32"
		}
	filter { "platforms:Win64", "action:vs*" }
		-- Pre Build Commands so that ImGui gets built before the Engine lib (Win64)
		prebuildcommands
		{
			"msbuild \"%{ProjectDirectories.ImGui}ImGui.vcxproj\" /p:Configuration=\"%{cfg.buildcfg} Win64\" /p:platform=x64"
		}
	filter { }

	end

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
	
	-- Only Include these if our current action is xcode4.
	filter { "action:xcode4" }
		files
		{
			"%{IncludeDirectories.spdlog}**.h",
			"%{IncludeDirectories.spdlog}**.cpp",
			"%{IncludeDirectories.spdlog}**.hpp"
		}
		externalincludedirs
		{
			"%{IncludeDirectories.spdlog}"
		}
	filter { }

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


	-- Ensures that all of the files outside of the source file don't have precompiled headers associated with them.
	filter { "files:not %{prj.location}/Source/**.h", "files:not {%prj.location}/Source/**.cpp", "files:not {%prj.location}/Source/**.hpp" }
		flags { "NoPCH" }
	filter { }



