#pragma once


#include "ImGuizmo.h"
#include "Source\Transform.h"

namespace Editor
{


	class TransformationWidget
	{
	public:
		TransformationWidget();
		TransformationWidget(const MathLib::Transform3D& transform);

		bool OnImGuiRender(const MathLib::Vector2& displayPos, const MathLib::Vector2& viewSize);

		void SetTransform(const MathLib::Transform3D& transform);
		void SetTransform(const MathLib::Transform2D& transform);

		const MathLib::Transform3D& GetTransform() const { return m_transform; }

		void SetEnabled(bool enabled) { m_enabled = enabled; }
		bool IsEnabled() const { return m_enabled; }

		void SetMovable(bool movable) { m_movable = movable; }
		bool IsMovable() const { return m_movable; }

	private:
		MathLib::Transform3D m_transform;
		bool m_enabled = true, m_movable = true;

	public:
		ImGuizmo::MODE m_widgetMode = ImGuizmo::MODE::LOCAL;
		ImGuizmo::OPERATION m_widgetOperation = ImGuizmo::OPERATION::TRANSLATE;
	};
}