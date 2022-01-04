#pragma once

#include "Material.h"
#include "Source\Matrix.h"
#include "Source\Vector.h"

namespace Engine
{
	struct MeshMaterialConstants
	{
		MathLib::Matrix4x4 c_objectToWorld = MathLib::Matrix4x4::Identity;
		MathLib::Vector3 c_objectPosition = MathLib::Vector3::Zero;

		MeshMaterialConstants()
			: c_objectToWorld(MathLib::Matrix4x4::Identity),
			c_objectPosition(0.0f, 0.0f, 0.0f) { }

	private:
		float pad;
	};

	class MeshComponent
	{
	public:
		explicit MeshComponent();
		~MeshComponent();

		const Material<MeshMaterialConstants>& GetMaterial() const;

		void SetMesh(class Mesh* mesh);
		class Mesh* GetMesh() const;

		void Bind() const;

	private:
		class Mesh* m_mesh;
		class VertexBuffer* m_vertexBuffer;
		class IndexBuffer* m_indexBuffer;
		Material<MeshMaterialConstants> m_material;
	};
}