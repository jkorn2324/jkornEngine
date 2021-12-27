#pragma once

#include "Source\Matrix.h"

namespace Engine
{
	class Camera
	{
	public:
		explicit Camera() : m_projectionMatrix(MathLib::Matrix4x4::Identity) { }
		explicit Camera(const MathLib::Matrix4x4& projMat) : m_projectionMatrix(projMat) { }
		virtual ~Camera() = default;

		void SetProjectionMatrix(const MathLib::Matrix4x4& matrix);
		const MathLib::Matrix4x4& GetProjectionMatrix() const;
		virtual MathLib::Matrix4x4 GetViewProjectionMatrix() const =0;

	protected:
		MathLib::Matrix4x4 m_projectionMatrix;
	};
}