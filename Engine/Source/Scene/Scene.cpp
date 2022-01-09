#include "EnginePCH.h"
#include "Scene.h"
#include "Entity.h"

#include "Components.h"
#include "Camera.h"
#include "ConstantBuffer.h"

namespace Engine
{

	// Static variables as there is only going to be one constant buffer.
	static ConstantBuffer* s_cameraConstantBuffer = nullptr;
	static CameraConstants s_cameraConstants = CameraConstants();
	static uint32_t s_scenesAllocated = 0;

	Scene::Scene()
		: m_entityRegistry(),
		m_markedForDestroyEntities(),
		m_camera(nullptr)
	{
		s_scenesAllocated++;

		if (s_cameraConstantBuffer == nullptr)
		{
			s_cameraConstantBuffer = ConstantBuffer::Create(
				&s_cameraConstants, sizeof(s_cameraConstants));
		}
	}

	Scene::~Scene()
	{
		s_scenesAllocated--;

		if (s_scenesAllocated <= 0
			&& s_cameraConstantBuffer != nullptr)
		{
			delete s_cameraConstantBuffer;
			s_cameraConstantBuffer = nullptr;
		}
	}

	void Scene::Update(const Timestep& ts)
	{
		// Destroys the entities if they are marked for destroy.
		{
			std::int32_t sizeOfVec = m_markedForDestroyEntities.size() - 1;
			while (sizeOfVec >= 0)
			{
				m_entityRegistry.destroy(m_markedForDestroyEntities[sizeOfVec]);
				sizeOfVec--;
			}
		}

		// Selects the view cameras based on a set of camera components.
		{
			auto entityView = m_entityRegistry.view<SceneCameraComponent, Transform3DComponent>();
			for (auto entity : entityView)
			{
				auto [cameraComponent, transformComponent]
					= entityView.get<SceneCameraComponent, Transform3DComponent>(entity);
				SceneCamera& sceneCam = cameraComponent.camera;

				// The camera's view matrix is from world to camera,
				// but the transform matrix is from object to world.
				auto matrix = transformComponent.GetTransformMatrix();
				matrix.Invert();
				sceneCam.SetViewMatrix(matrix);
				sceneCam.UpdateProjectionMatrix();

				if (cameraComponent.mainCamera)
				{
					m_camera = &cameraComponent.camera;
					break;
				}
			}
		}
	}

	void Scene::Render()
	{
		{
			// Sets the camera constants along with buffers.
			if (m_camera != nullptr)
			{
				auto mat = m_camera->GetViewMatrix();
				mat.Invert();

				s_cameraConstants.c_cameraPosition = mat.GetTranslation();
				s_cameraConstants.c_viewProjection =
					m_camera->GetViewProjectionMatrix();

				s_cameraConstantBuffer->SetData(&s_cameraConstants,
					sizeof(s_cameraConstants));
				s_cameraConstantBuffer->Bind(0,
					Engine::ConstantBufferFlags::VERTEX_SHADER | Engine::ConstantBufferFlags::PIXEL_SHADER);
			}
		}

		{
			// TODO: Draw all sprite components
		}
	}

	Camera* Scene::GetCamera() const
	{
		return m_camera;
	}

	Entity Scene::CreateEntity()
	{
		Entity createdEntity = Entity(m_entityRegistry.create(), this);
		return createdEntity;
	}

	void Scene::DestroyEntity(const Entity& entity)
	{
		const auto& find = std::find(m_markedForDestroyEntities.begin(), 
			m_markedForDestroyEntities.end(), entity.m_entity);

		if (find == m_markedForDestroyEntities.end())
		{
			m_markedForDestroyEntities.push_back(entity.m_entity);
		}
	}
}
