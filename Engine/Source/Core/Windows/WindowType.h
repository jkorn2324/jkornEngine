#pragma once

namespace Engine
{

    /**
     * @brief The window type.
     */
    enum class WindowAPIType
    {
        WINDOW_NONE,
        WINDOW_GLFW
    };

    // TODO: Get the current type from macros.
    constexpr WindowAPIType g_ActiveWindowAPIType = WindowAPIType::WINDOW_GLFW;
}
