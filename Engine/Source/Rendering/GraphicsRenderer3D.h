#pragma once

#include "Source\Matrix.h"

namespace Engine
{

	struct PointLightComponent;
	struct DirectionalLightComponent;

	static const uint32_t MAX_POINT_LIGHTS = 32;

	class GraphicsRenderer3D
	{
	public:
		static void Init();
		static void Release();

		static void DrawMesh(const MathLib::Vector3& position, class Mesh& mesh);
		static void DrawMesh(const MathLib::Matrix4x4& transformMatrix, class Mesh& mesh);
		static void DrawMesh(const MathLib::Matrix4x4& transformMatrix, class Mesh& mesh,
			const class Material& material);

		static void DrawCube(const MathLib::Matrix4x4& transformMatrix,
			const class Material& material);
		static void DrawCube(const MathLib::Matrix4x4& transformMatrix);

		static void SetAmbientLight(const MathLib::Vector3& color);
		static bool AddPointLight(const MathLib::Vector3& position,
			const PointLightComponent& pointLight);
		static void SetDirectionalLight(const MathLib::Vector3& direction,
			const DirectionalLightComponent& directionalLight);

	private:
		static void BindLights();

		friend class GraphicsRenderer;
	};
}