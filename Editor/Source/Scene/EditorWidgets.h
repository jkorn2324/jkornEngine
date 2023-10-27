#pragma once


#include "ImGuizmo.h"
#include "Transform.h"

namespace Editor
{


	class TransformationWidget
	{
		enum TransformationWidgetEnabledType
		{
			Type_Enabled = 1 << 0,
			Type_Movable = 1 << 1,
			Type_Using = 1 << 2,
			Type_Over = 1 << 3
		};
	public:
		TransformationWidget();
		TransformationWidget(const MathLib::Transform3D& transform);

		bool OnImGuiRender(const MathLib::Vector2& displayPos, const MathLib::Vector2& viewSize);

		void SetTransform(const MathLib::Transform3D& transform);
		void SetTransform(const MathLib::Transform2D& transform);

		const MathLib::Transform3D& GetTransform() const { return m_transform; }

		bool IsMouseUsing() const { return m_transformationWidgetEnabled & Type_Using; }
		bool IsMouseOver() const { return m_transformationWidgetEnabled & Type_Over; }

		void SetEnabled(bool enabled) { SetTransformationWidgetFlag(enabled, Type_Enabled); }
		bool IsEnabled() const { return m_transformationWidgetEnabled & Type_Enabled; }

		void SetMovable(bool movable) { SetTransformationWidgetFlag(movable, Type_Movable); }
		bool IsMovable() const { return m_transformationWidgetEnabled & Type_Movable; }


	private:
		void SetMouseOver(bool value)
		{
			SetTransformationWidgetFlag(value, Type_Over);
		}

		void SetMouseUsing(bool value)
		{
			SetTransformationWidgetFlag(value, Type_Using);
		}

		void SetTransformationWidgetFlag(bool type, uint8_t flag)
		{
			if (type)
			{
				m_transformationWidgetEnabled |= flag;
			}
			else
			{
				m_transformationWidgetEnabled &= ~flag;
			}
		}

	private:
		MathLib::Transform3D m_transform;
		uint8_t m_transformationWidgetEnabled = Type_Enabled | Type_Movable;

	public:
		ImGuizmo::MODE m_widgetMode = ImGuizmo::MODE::LOCAL;
		ImGuizmo::OPERATION m_widgetOperation = ImGuizmo::OPERATION::TRANSLATE;
	};
}
