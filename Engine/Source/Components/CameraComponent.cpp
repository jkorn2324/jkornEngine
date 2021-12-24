#include "EnginePCH.h"
#include "CameraComponent.h"

#include "Rendering\ConstantBuffer.h"
#include "Source\Transform.h"
#include "Entities\Entity.h"

namespace Engine
{
	using Transform3D = MathLib::Transform3D;
	using Mat4x4 = MathLib::Matrix4x4;

	CameraComponent::CameraComponent(Entity* entity)
		: Component(entity),
		m_cameraConstants(),
		m_constantBuffer(nullptr)
	{
		// The View Matrix.
		m_viewMatrix = MathLib::Matrix4x4::CreateOrtho(1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 100.0f);
		m_cameraConstants.c_cameraPos = MathLib::Vector3::Zero;
		m_constantBuffer = new ConstantBuffer(
			&m_cameraConstants, sizeof(m_cameraConstants));
	}

	CameraComponent::~CameraComponent()
	{
		delete m_constantBuffer;
	}

	void CameraComponent::Update(float deltaTime)
	{
		// Updated camera position and view matrix.
		Mat4x4 worldToCameraMat = GetOwner()->GetTransform().GetTransformMatrix();
		m_cameraConstants.c_cameraPos = worldToCameraMat.GetTranslation();
		// Transforms from camera to world to world to camera.
		worldToCameraMat.Invert();
		m_cameraConstants.c_viewProjection = worldToCameraMat * m_viewMatrix;
		m_constantBuffer->Update(&m_cameraConstants, 
			sizeof(m_cameraConstants));
	}
}