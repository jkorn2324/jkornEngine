#include "EditorPCH.h"
#include "EditorWidgets.h"

#include "EditorCamera.h"
#include "EditorSceneManager.h"

#include <imgui.h>

namespace Editor
{
    TransformationWidget::TransformationWidget()
        : m_transform()
    {
    }

    TransformationWidget::TransformationWidget(const MathLib::Transform3D& transform)
        : m_transform(transform)
    {
    }

    bool TransformationWidget::OnImGuiRender(const MathLib::Vector2& displayPos,
        const MathLib::Vector2& displayBounds)
    {
        if (!IsEnabled())
        {
            SetMouseUsing(false);
            return false;
        }
        Editor::EditorCamera& camera
            = EditorSceneManager::GetEditorCamera();

        ImGuiIO& io = ImGui::GetIO();

        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect(displayPos.x, displayPos.y, displayBounds.x, displayBounds.y);

        MathLib::Matrix4x4 transformMatrix
            = m_transform.GetTransformMatrix();
        
        bool manipulated = ImGuizmo::Manipulate(
            reinterpret_cast<const float*>(&camera.GetViewMatrix()),
                reinterpret_cast<const float*>(&camera.GetProjectionMatrix()),
                    m_widgetOperation, m_widgetMode, (float*)transformMatrix.matrix, nullptr, nullptr);

        SetMouseOver(ImGuizmo::IsOver());
        if (!IsMovable()) 
        {
            SetMouseUsing(false);
            return false;
        }
        SetMouseUsing(ImGuizmo::IsUsing());

        if (manipulated)
        {
            if (m_transform.HasParentTransformMatrix())
            {
                MathLib::Matrix4x4 inverted = MathLib::Matrix4x4::Invert(m_transform.GetParentTransformMatrix());
                transformMatrix = inverted * transformMatrix;
            }

            MathLib::Vector3 rotation;
            MathLib::Vector3 position;
            MathLib::Vector3 scale;

            ImGuizmo::DecomposeMatrixToComponents((float*)&transformMatrix, 
                (float*)&position, (float*)&rotation, (float*)&scale);

            MathLib::Vector3 deltaRotation = rotation - m_transform.GetLocalEulerAngles(true);
            m_transform.SetLocalPosition(position);
            m_transform.SetLocalScale(scale);
            m_transform.SetLocalEulerAngles(
                m_transform.GetLocalEulerAngles(true) + deltaRotation, true);
        }
        return manipulated;
    }

    void TransformationWidget::SetTransform(const MathLib::Transform3D& transform)
    {
        m_transform = transform;
    }
    
    void TransformationWidget::SetTransform(const MathLib::Transform2D& transform)
    {
        m_transform = transform;
    }
}
