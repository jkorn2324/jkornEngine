
IncludeDirectories = {}
BuildDirectories = {}
ProjectDirectories = {}
LibraryNames = {}

LibrariesLocation = "%{wks.location}/Engine/Libraries"

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

-- The vulkan sdk reference
VulkanSDKWindows = os.getenv("VULKAN_SDK")

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

-- ================================== MAC DEFINITIONS =============================--

-- TODO: In a setup script, install vulkan sdk w/ binaries to a localized path
VulkanSDKMacOS = "/Users/jkorn/VulkanSDK/1.3.268.1/macOS"

-- DXC
IncludeDirectories["dxc_macos"] = "%{VulkanSDKMacOS}/include/dxc/"
BuildDirectories["dxc_macos"] = "%{VulkanSDKMacOS}/lib/"
LibraryNames["dxc_macos"] = "%{BuildDirectories.dxc_macos}libdxcompiler.dylib"

-- SPIR-V Cross
IncludeDirectories["spirvcross_macos"] = "%{VulkanSDKMacOS}/include/spirv_cross/"
BuildDirectories["spirvcross_macos"] = "%{VulkanSDKMacOS}/lib/"

LibraryNames["spirvcross_macos_core"] = "%{BuildDirectories.spirvcross_macos}libspirv-cross-core.a"
LibraryNames["spirvcross_macos_msl"] = "%{BuildDirectories.spirvcross_macos}libspirv-cross-msl.a" -- Mlsl
LibraryNames["spirvcross_macos_hlsl"] = "%{BuildDirectories.spirvcross_macos}libspirv-cross-hlsl.a" -- Hlsl

-- ShaderC
IncludeDirectories["shaderc_macos"] = "%{VulkanSDKMacOS}/include/shaderc/"
BuildDirectories["shaderc_macos"] = "%{VulkanSDKMacOS}/lib/"
LibraryNames["shaderc_macos"] = "%{BuildDirectories.shaderc_macos}libshaderc_shared.a"


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
            "%{BuildDirectories.dxc_macos}",
            "%{BuildDirectories.spirvcross_macos}",
            "%{BuildDirectories.shaderc_macos}"
        }

        -- Emulate UUID must be defined on linux/macos platforms
        defines
        {
            "__EMULATE_UUID"
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