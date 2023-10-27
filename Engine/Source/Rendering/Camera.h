#pragma once

#include "Matrix.h"

namespace Engine
{
	class Camera
	{
	public:
		explicit Camera() 
			: m_projectionMatrix(MathLib::Matrix4x4::Identity),
			  m_viewMatrix(MathLib::Matrix4x4::Identity) { }
		virtual ~Camera() = default;

		const MathLib::Matrix4x4& GetViewMatrix() const { return m_viewMatrix; }
		const MathLib::Matrix4x4& GetProjectionMatrix() const { return m_projectionMatrix; }
		MathLib::Matrix4x4 GetViewProjectionMatrix() const 
		{ 
			return m_viewMatrix * m_projectionMatrix; 
		}

		MathLib::Vector3 ScreenToWorld(const MathLib::Vector2& screenPos);
		MathLib::Vector2 WorldToScreen(const MathLib::Vector3& worldPos);

	protected:
		// The world to camera matrix.
		MathLib::Matrix4x4 m_viewMatrix;
		// The projection matrix.
		MathLib::Matrix4x4 m_projectionMatrix;
	};
}
