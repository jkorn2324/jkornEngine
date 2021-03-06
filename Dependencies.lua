IncludeDirectories = {}

IncludeDirectories["DirectXTK"] = "%{wks.location}/Engine/Libraries/DirectXTK/Inc/"
IncludeDirectories["entt"] = "%{wks.location}/Engine/Libraries/entt/include/"
IncludeDirectories["glfw"] = "%{wks.location}/Engine/Libraries/glfw/include/"
IncludeDirectories["ImGui"] = "%{wks.location}/Engine/Libraries/ImGui/"
IncludeDirectories["spdlog"] = "%{wks.location}/Engine/Libraries/spdlog/include/"
IncludeDirectories["rapidjson"] = "%{wks.location}/Engine/Libraries/rapidjson/include/"
IncludeDirectories["ImGuizmo"] = "%{wks.location}/Engine/Libraries/ImGuizmo/"

LibraryDirectories = {}

-- TODO: Add proper library directories.
LibraryDirectories["MathLib"] = "%{wks.location}/MathLib/"
LibraryDirectories["DirectXTK"] = "%{wks.location}/Engine/Libraries/DirectXTK/"
LibraryDirectories["glfw"] = "%{wks.location}/Engine/Libraries/Builds/glfw/"
LibraryDirectories["spdlog"] = "%{wks.location}/Engine/Libraries/Builds/spdlog/"
LibraryDirectories["ImGui"] = "%{wks.location}/Engine/Libraries/Builds/ImGui/"

LibraryNames = {}

LibraryNames["MathLib"] = "MathLib.lib"
LibraryNames["DirectXTK"] = "DirectXTK.lib"
LibraryNames["glfw"] = "glfw.lib"
LibraryNames["spdlog"] = "spdlogd.lib"
LibraryNames["ImGui"] = "ImGui.lib"