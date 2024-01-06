
IncludeDirectories = {}
BuildDirectories = {}
ProjectDirectories = {}
LibraryNames = {}

LibrariesLocation = "%{wks.location}/Engine/Libraries"
ExternalBinariesLocation = "%{wks.location}/External/Binaries"

-- The vulkan sdk install location path
VulkanSDKLocation = "%{ExternalBinariesLocation}/vulkansdk/install/"

-- Cross Platform Dependencies

-- Math Lib Includes
IncludeDirectories["MathLib"] = "%{wks.location}/MathLib/Source/"
ProjectDirectories["MathLib"] = "%{wks.location}/MathLib/"
BuildDirectories["MathLib"] = "%{wks.location}/MathLib/Builds/"
LibraryNames["MathLib"] = "MathLib"

-- Entt Includes
IncludeDirectories["entt"] = "%{LibrariesLocation}/entt/single_include/"

-- Glfw Includes
IncludeDirectories["glfw"] = "%{LibrariesLocation}/glfw/include/"
ProjectDirectories["glfw"] = "%{LibrariesLocation}/glfw/"
BuildDirectories["glfw"] = "%{LibrariesLocation}/Builds/glfw/Builds/"
LibraryNames["glfw"] = "glfw"

-- ImGui Includes
IncludeDirectories["ImGui"] = "%{LibrariesLocation}/ImGui/"
BuildDirectories["ImGui"] = "%{LibrariesLocation}/Builds/ImGui/Builds/"
ProjectDirectories["ImGui"] = "%{LibrariesLocation}/ImGui/"
LibraryNames["ImGui"] = "ImGui"

-- SPDLog Includes
IncludeDirectories["spdlog"] = "%{LibrariesLocation}/spdlog/include/"
BuildDirectories["spdlog"] = "%{LibrariesLocation}/Builds/spdlog/Builds/"
LibraryNames["spdlog"] = "spdlogd.lib"

-- RapidJson Includes
IncludeDirectories["rapidjson"] = "%{LibrariesLocation}/rapidjson/include/"

-- ImGuizmo Includes
IncludeDirectories["ImGuizmo"] = "%{LibrariesLocation}/ImGuizmo/"

-- ================================== WINDOWS DEPENDENCIES =============================--

-- TODO: In a setup script, install libfbxsdk w/ binaries to a localized path
FBXSDKPath_Windows = "C:/Program Files/Autodesk/FBX/FBX SDK"
FBXSDKVersion_Windows = "2020.3.4"
FBXSDKVersionPath_Windows = "%{FBXSDKPath_Windows}/%{FBXSDKVersion_Windows}"

-- FBX SDK DEFINITIONS
IncludeDirectories["fbxsdk_windows"] = "%{FBXSDKVersionPath_Windows}/include/"
BuildDirectories["fbxsdk_windows"] = "%{FBXSDKVersionPath_Windows}/lib/"
LibraryNames["fbxsdk_windows"] = "libfbxsdk.lib"

-- DIRECTX DEFINITIONS
LibraryNames["d3d11"] = "d3d11.lib"

-- DIRECTXTK DEFINITIONS
IncludeDirectories["DirectXTK"] = "%{LibrariesLocation}/DirectXTK/Inc/"
BuildDirectories["DirectXTK"] = "%{LibrariesLocation}/DirectXTK/"
ProjectDirectories["DirectXTK"] = "%{LibrariesLocation}/DirectXTK/"
LibraryNames["DirectXTK"] = "DirectXTK.lib"

-- VulkanSDK
BuildDirectories["vulkansdk_win64"] = "%{VulkanSDKLocation}/Lib/"
BuildDirectories["vulkansdk_win32"] = "%{VulkanSDKLocation}/Lib32/"

-- DXC
IncludeDirectories["dxc_windows"] = "%{VulkanSDKLocation}/Include/dxc/"
LibraryNames["dxc_win64_release"] = "%{BuildDirectories.vulkansdk_win64}dxcompiler.lib"
LibraryNames["dxc_win64_debug"] = "%{BuildDirectories.vulkansdk_win64}dxcompilerd.lib"
LibraryNames["dxc_win32_release"] = "%{BuildDirectories.vulkansdk_win32}dxcompiler.lib"
LibraryNames["dxc_win32_debug"] = "%{BuildDirectories.vulkansdk_win32}dxcompilerd.lib"

-- SPIR-V Cross Core
IncludeDirectories["spirvcross_windows"] = "%{VulkanSDKLocation}/Include/spirv_cross/"
LibraryNames["spirvcross_win64_release_core"] = "%{BuildDirectories.vulkansdk_win64}spirv-cross-core.lib"
LibraryNames["spirvcross_win64_debug_core"] = "%{BuildDirectories.vulkansdk_win64}spirv-cross-cored.lib"
LibraryNames["spirvcross_win32_release_core"] = "%{BuildDirectories.vulkansdk_win32}spirv-cross-core.lib"
LibraryNames["spirvcross_win32_debug_core"] = "%{BuildDirectories.vulkansdk_win32}spirv-cross-cored.lib"

-- SPIR-V Cross Msl
LibraryNames["spirvcross_win64_release_msl"] = "%{BuildDirectories.vulkansdk_win64}spirv-cross-msl.lib"
LibraryNames["spirvcross_win64_debug_msl"] = "%{BuildDirectories.vulkansdk_win64}spirv-cross-msld.lib"
LibraryNames["spirvcross_win32_release_msl"] = "%{BuildDirectories.vulkansdk_win32}spirv-cross-msl.lib"
LibraryNames["spirvcross_win32_debug_msl"] = "%{BuildDirectories.vulkansdk_win32}spirv-cross-msld.lib"

-- SPIR-V Cross Hlsl
LibraryNames["spirvcross_win64_release_hlsl"] = "%{BuildDirectories.vulkansdk_win64}spirv-cross-hlsl.lib"
LibraryNames["spirvcross_win64_debug_hlsl"] = "%{BuildDirectories.vulkansdk_win64}spirv-cross-hlsld.lib"
LibraryNames["spirvcross_win32_release_hlsl"] = "%{BuildDirectories.vulkansdk_win32}spirv-cross-hlsl.lib"
LibraryNames["spirvcross_win32_debug_hlsl"] = "%{BuildDirectories.vulkansdk_win32}spirv-cross-hlsld.lib"

-- ShaderC
IncludeDirectories["shaderc_windows"] = "%{VulkanSDKLocation}/Include/"
LibraryNames["shaderc_win64_release"] = "%{BuildDirectories.vulkansdk_win64}shaderc_shared.lib"
LibraryNames["shaderc_win64_debug"] = "%{BuildDirectories.vulkansdk_win64}shaderc_sharedd.lib"
LibraryNames["shaderc_win32_release"] = "%{BuildDirectories.vulkansdk_win32}shaderc_shared.lib"
LibraryNames["shaderc_win32_debug"] = "%{BuildDirectories.vulkansdk_win32}shaderc_sharedd.lib"

-- ================================== MAC DEPENDENCIES =============================--

-- VulkanSDK
BuildDirectories["vulkansdk_macos"] = "%{VulkanSDKLocation}/lib/"

-- DXC
IncludeDirectories["dxc_macos"] = "%{VulkanSDKLocation}/include/dxc/"
LibraryNames["dxc_macos"] = "%{BuildDirectories.vulkansdk_macos}libdxcompiler.dylib"

-- SPIR-V Cross Core
IncludeDirectories["spirvcross_macos"] = "%{VulkanSDKLocation}/include/spirv_cross/"
LibraryNames["spirvcross_macos_core"] = "%{BuildDirectories.vulkansdk_macos}libspirv-cross-core.a"

-- SPIR-V Cross Msl
LibraryNames["spirvcross_macos_msl"] = "%{BuildDirectories.vulkansdk_macos}libspirv-cross-msl.a" -- Mlsl

-- SPIR-V Cross Hlsl
LibraryNames["spirvcross_macos_hlsl"] = "%{BuildDirectories.vulkansdk_macos}libspirv-cross-hlsl.a" -- Hlsl

-- ShaderC
IncludeDirectories["shaderc_macos"] = "%{VulkanSDKLocation}/include/shaderc/"
LibraryNames["shaderc_macos"] = "%{BuildDirectories.vulkansdk_macos}libshaderc_shared.a"


-- Includes the dxc compiler dependencies
function include_shadertool_dependencies()

    -- Includes dxc & spirv_cross to the platforms
    filter { "action:xcode*", "platforms:MacOS" }
        
        externalincludedirs
        {
            "%{IncludeDirectories.dxc_macos}",
            "%{IncludeDirectories.spirvcross_macos}",
            "%{IncludeDirectories.shaderc_macos}"
        }

        libdirs
        {
            "%{BuildDirectories.vulkansdk_macos}"
        }

        -- Emulate UUID must be defined on linux/macos platforms
        defines
        {
            "__EMULATE_UUID"
        }

    -- If Platform is Windows, just include dxc
    filter { "action:vs*", "platforms:Win64 or Win32" }
        includedirs
        {
            "%{IncludeDirectories.dxc_windows}",
            "%{IncludeDirectories.spirvcross_windows}",
            "%{IncludeDirectories.shaderc_windows}"
        }
    filter { }

    -- If platform is Windows x64
    filter { "action:vs*", "platforms:Win64" }
        libdirs
        {
            "%{BuildDirectories.vulkansdk_win64}"
        }
    -- If platform is Windows x86
    filter { "action:vs*", "platforms:Win32" }
        libdirs
        {
            "%{BuildDirectories.vulkansdk_win32}"
        }
    filter { }

    -- If platform is Windows x64, Debug
    filter { "action:vs*", "platforms:Win64", "configurations:Debug" }
        links
        {
            "%{LibraryNames.dxc_win64_debug}",
            "%{LibraryNames.spirvcross_win64_debug_core}",
            "%{LibraryNames.spirvcross_win64_debug_msl}",
            "%{LibraryNames.spirvcross_win64_debug_hlsl}",
            "%{LibraryNames.shaderc_win64_debug}"
        }

    -- If platform is Windows x64, Release
    filter { "action:vs*", "platforms:Win32", "configurations:Release" }
        links
        {
            "%{LibraryNames.dxc_win64_release}",
            "%{LibraryNames.spirvcross_win64_release_core}",
            "%{LibraryNames.spirvcross_win64_release_msl}",
            "%{LibraryNames.spirvcross_win64_release_hlsl}",
            "%{LibraryNames.shaderc_win64_release}"
        }
    -- If platform is Windows x86 (32 Bit), Debug
    filter { "action:vs*", "platforms:Win64", "configurations:Debug" }
        links
        {
            "%{LibraryNames.dxc_win32_debug}",
            "%{LibraryNames.spirvcross_win32_debug_core}",
            "%{LibraryNames.spirvcross_win32_debug_msl}",
            "%{LibraryNames.spirvcross_win32_debug_hlsl}",
            "%{LibraryNames.shaderc_win32_debug}"
        }
    -- If platform is Windows x86 (32 Bit), Release
    filter { "action:vs*", "platforms:Win32", "configurations:Release" }
        links
        {
            "%{LibraryNames.dxc_win32_release}",
            "%{LibraryNames.spirvcross_win32_release_core}",
            "%{LibraryNames.spirvcross_win32_release_msl}",
            "%{LibraryNames.spirvcross_win32_release_hlsl}",
            "%{LibraryNames.shaderc_win32_release}"
        }
    filter { }
end

-- Includes apple frameworks
function include_apple_frameworks()
    	-- Includes the Frameworks to the Project if we are on macos
	filter { "action:xcode*", "platforms:MacOS" }
        links
        {
            "CoreFoundation.framework",
            "Cocoa.framework",
            "IOKit.framework",
            "CoreVideo.framework",
            "AppKit.framework"
        }

        buildoptions
        {
            "-framework CoreFoundation",
            "-framework Cocoa",
            "-framework IOKit",
            "-framework CoreVideo",
            "-framework AppKit"
        }

        linkoptions
        {
            "-framework CoreFoundation",
            "-framework Cocoa",
            "-framework IOKit",
            "-framework CoreVideo",
            "-framework AppKit"
        }
    filter { }
end

function include_metal_frameworks()
	-- TODO: Need to integrate options for different ios/macos versions
	filter { "platforms:MacOS", "options:graphicsapi=metal", "action:xcode*" }

		links
        {
			"Metal.framework",
			"QuartzCore.framework",
			"Foundation.framework"
        }

        buildoptions
        {
            "-framework Metal",
            "-framework QuartzCore",
            "-framework Foundation"
        }

        linkoptions
        {
            "-framework Metal",
            "-framework QuartzCore",
            "-framework Foundation"
        }
		
	filter { }
end