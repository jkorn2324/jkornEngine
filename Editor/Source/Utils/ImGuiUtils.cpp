#include "EditorPCH.h"
#include "ImGuiUtils.h"

#include <imgui_internal.h>

namespace Editor
{

	void ImGuiUtils::DrawImage(const void* imageID,
		const ImVec2& posOffset, const ImVec2& size)
	{
        DrawImage(imageID, posOffset, size,
            { 0.0f, 0.0f }, { 1.0f, 1.0f });
	}

    void ImGuiUtils::DrawImage(const void* imageID, const ImVec2& posOffset,
        const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1)
    {
        DrawImage(imageID, posOffset, size, uv0,
            uv1, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 0.0f });
    }
    
    void ImGuiUtils::DrawImage(const void* imageID, const ImVec2& posOffset, const ImVec2& size, 
        const ImVec2& uv0, const ImVec2& uv1, const ImVec4& tintColor, const ImVec4& borderColor)
    {
        ImGuiContext* context = ImGui::GetCurrentContext();
        ImGuiWindow* window = context->CurrentWindow;
        if (window->SkipItems)
            return;

        ImVec2 newPos = { window->DC.CursorPos.x + posOffset.x,
            window->DC.CursorPos.y + posOffset.y };
        ImRect bb(newPos, { newPos.x + size.x, newPos.y + size.y });
        if (borderColor.w > 0.0f)
        {
            bb.Max.x += 2.0f;
            bb.Max.y += 2.0f;
        }
        ImGui::ItemSize(bb);
        if (!ImGui::ItemAdd(bb, 0))
            return;

        if (borderColor.w > 0.0f)
        {
            window->DrawList->AddRect(bb.Min, bb.Max, ImGui::GetColorU32(borderColor), 0.0f);
            window->DrawList->AddImage((void*)imageID, { bb.Min.x + 1.0f, bb.Min.y + 1.0f },
                { bb.Max.x - 1.0f, bb.Min.y - 1.0f }, uv0, uv1, ImGui::GetColorU32(tintColor));
        }
        else
        {
            window->DrawList->AddImage((void*)imageID, bb.Min, bb.Max, uv0, uv1, ImGui::GetColorU32(tintColor));
        }
    }
}
