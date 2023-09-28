
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
LibraryNames["glfw"] = "glfw.lib"

IncludeDirectories["ImGui"] = "%{LibrariesLocation}/ImGui/"
BuildDirectories["ImGui"] = "%{LibrariesLocation}/Builds/ImGui/Builds/"
ProjectDirectories["ImGui"] = "%{LibrariesLocation}/ImGui/"
LibraryNames["ImGui"] = "ImGui.lib"

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