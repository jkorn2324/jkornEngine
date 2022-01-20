#pragma once

#include "Source\Matrix.h"

namespace Engine
{

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
	};
}