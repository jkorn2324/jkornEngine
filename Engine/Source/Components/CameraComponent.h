#pragma once

#include "Component.h"

#include "Source\Matrix.h"
#include "Source\Vector.h"

namespace Engine
{

	// TODO: Need to set the active Camera Component

	class CameraComponent : public Component
	{
	public:
		struct CameraConstants
		{
			MathLib::Matrix4x4 c_viewProjection;
			MathLib::Vector3 c_cameraPos;

		private:
			float pad;
		};

	public:
		explicit CameraComponent(class Entity* entity);
		~CameraComponent();

		void Update(float deltaTime) override;

	public:
		class ConstantBuffer* m_constantBuffer;
		
	private:
		CameraConstants m_cameraConstants;
		MathLib::Matrix4x4 m_viewMatrix;
	};
}