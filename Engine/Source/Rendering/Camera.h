#pragma once

#include "Source\Matrix.h"

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
		MathLib::Matrix4x4 GetViewProjectionMatrix() const { return m_viewMatrix * m_projectionMatrix; }

	protected:
		// The world to camera matrix.
		MathLib::Matrix4x4 m_viewMatrix;
		// The projection matrix.
		MathLib::Matrix4x4 m_projectionMatrix;
	};
}