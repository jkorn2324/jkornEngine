
IncludeDirectories = {}
BuildDirectories = {}
ProjectDirectories = {}
LibraryNames = {}

LibrariesLocation = "%{wks.location}/Engine/Libraries"

-- Cross Platform Dependencies

IncludeDirectories["MathLib"] = "%{wks.location}/MathLib/Source/"
ProjectDirectories["MathLib"] = "%{wks.location}/MathLib/"
BuildDirectories["MathLib"] = "%{wks.location}/MathLib/Builds/"
LibraryNames["MathLib"] = "MathLib"

IncludeDirectories["entt"] = "%{LibrariesLocation}/entt/single_include/"

IncludeDirectories["glfw"] = "%{LibrariesLocation}/glfw/include/"
ProjectDirectories["glfw"] = "%{LibrariesLocation}/glfw/"
BuildDirectories["glfw"] = "%{LibrariesLocation}/Builds/glfw/Builds/"
LibraryNames["glfw"] = "glfw"

IncludeDirectories["ImGui"] = "%{LibrariesLocation}/ImGui/"
BuildDirectories["ImGui"] = "%{LibrariesLocation}/Builds/ImGui/Builds/"
ProjectDirectories["ImGui"] = "%{LibrariesLocation}/ImGui/"
LibraryNames["ImGui"] = "ImGui"

IncludeDirectories["spdlog"] = "%{LibrariesLocation}/spdlog/include/"
BuildDirectories["spdlog"] = "%{LibrariesLocation}/Builds/spdlog/Builds/"
LibraryNames["spdlog"] = "spdlogd.lib"

IncludeDirectories["rapidjson"] = "%{LibrariesLocation}/rapidjson/include/"

IncludeDirectories["ImGuizmo"] = "%{LibrariesLocation}/ImGuizmo/"

-- Specific Platform Dependencies --

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

-- METAL DEFINITIONS
IncludeDirectories["metalcpp_macos12_ios15"] = "%{LibrariesLocation}/metal-cpp/metal-cpp-macos12-ios15/Metal/"
-- QUARTZCORE DEFINITIONS
IncludeDirectories["quartzcore_macos12_ios15"] = "%{LibrariesLocation}/metal-cpp/metal-cpp-macos12-ios15/QuartzCore/"
-- FOUNDATION DEFINITIONS
IncludeDirectories["foundation_macos12_ios15"] = "%{LibrariesLocation}/metal-cpp/metal-cpp-macos12-ios15/Foundation/"

-- Includes apple frameworks
function include_apple_frameworks()
    	-- Includes the Frameworks to the Project if we are on macos
	filter { "action:xcode*", "platforms:MacOS" }
        links
        {
            "CoreFoundation.framework",
            "Cocoa.framework",
            "IOKit.framework",
            "CoreVideo.framework"
        }

        buildoptions
        {
            "-framework Cocoa",
            "-framework IOKit",
            "-framework CoreFoundation",
            "-framework CoreVideo"
        }

        linkoptions
        {
            "-framework Cocoa",
            "-framework IOKit",
            "-framework CoreFoundation",
            "-framework CoreVideo"
        }
    filter { }

    -- Only include these if we are using metal.
    filter { "action:xcode*", "platforms:MacOS", "options:graphicsapi=metal"}
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