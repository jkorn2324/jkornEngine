#pragma once

#include <imgui.h>

namespace Editor
{

	class ImGuiUtils
	{
	public:
		static void DrawImage(const void* imageID, const ImVec2& posOffset,
			const ImVec2& size);
		static void DrawImage(const void* imageID, const ImVec2& posOffset,
			const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1);

		static void DrawImage(const void* imageID, const ImVec2& posOffset,
			const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, 
			const ImVec4& tintColor, const ImVec4& borderColor);
	};
}