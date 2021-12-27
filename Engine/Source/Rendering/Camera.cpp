#include "EnginePCH.h"
#include "Camera.h"

namespace Engine
{
	void Camera::SetProjectionMatrix(const MathLib::Matrix4x4& matrix)
	{
		m_projectionMatrix = matrix;
	}

	const MathLib::Matrix4x4& Camera::GetProjectionMatrix() const
	{
		return m_projectionMatrix;
	}
}