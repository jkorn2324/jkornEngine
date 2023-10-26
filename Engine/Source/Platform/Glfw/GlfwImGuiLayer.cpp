#include "EnginePCH.h"
#include "PlatformImGuiLayer.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <Glfw/glfw3.h>

#include "RenderingAPI.h"
#include "Application.h"
#include "Window.h"

namespace Engine
{

    template<>
    constexpr bool Platform::Internals::IsDefined<WindowAPIType, WindowAPIType::WINDOW_GLFW>() { return true; }

    template<>
    void Platform::Internals::OnLayerAddedImpl<WindowAPIType, WindowAPIType::WINDOW_GLFW>()
    {
        Window& window = Application::Get().GetWindow();
        // Initializes GLFW for DirectX11 & Metal
#if defined(GRAPHICS_API_DIRECTX11) || defined(GRAPHICS_API_METAL)
        ImGui_ImplGlfw_InitForOther(window.GetRawWindowPtr<GLFWwindow>(), true);
#endif
    }

    template<>
    void Platform::Internals::OnLayerRemovedImpl<WindowAPIType, WindowAPIType::WINDOW_GLFW>()
    {
        ImGui_ImplGlfw_Shutdown();
    }

    template<>
    void Platform::Internals::BeginFrameImpl<WindowAPIType, WindowAPIType::WINDOW_GLFW>()
    {
        ImGui_ImplGlfw_NewFrame();
    }

    template<>
    void Platform::Internals::EndFrameImpl<WindowAPIType, WindowAPIType::WINDOW_GLFW>()
    {
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            Window& window = Application::Get().GetWindow();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(window.GetRawWindowPtr<GLFWwindow>());
        }
    }
}
