#include "EnginePCH.h"
#include "MeshComponent.h"

#include "Mesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace Engine
{
	MeshComponent::MeshComponent()
		: m_material(),
		m_vertexBuffer(nullptr),
		m_indexBuffer(nullptr),
		m_mesh(nullptr)
	{
	}
	
	MeshComponent::~MeshComponent()
	{
		delete m_vertexBuffer;
		delete m_indexBuffer;
	}

	const Material<MeshMaterialConstants>& MeshComponent::GetMaterial() const
	{
		return m_material;
	}
	
	void MeshComponent::SetMesh(Mesh* mesh)
	{
		if (mesh != nullptr)
		{
			if (m_vertexBuffer != nullptr)
			{
				m_vertexBuffer = VertexBuffer::Create(
					mesh->GetVertices(), mesh->GetNumVertices(), sizeof(MeshVertex));
			}
			if (m_indexBuffer != nullptr)
			{
				m_indexBuffer = IndexBuffer::Create(
					mesh->GetIndices(), mesh->GetNumIndices(), sizeof(MeshVertex));
			}
		}
		m_mesh = mesh;
	}
	
	Mesh* MeshComponent::GetMesh() const
	{
		return m_mesh;
	}
	
	void MeshComponent::Bind() const
	{
		if (m_mesh != nullptr)
		{
			m_vertexBuffer->SetData(m_mesh->GetVertices(),
				m_mesh->GetNumVertices(), sizeof(MeshVertex));
			m_indexBuffer->SetData(m_mesh->GetIndices(),
				m_mesh->GetNumIndices(), sizeof(uint32_t));
		}
	}
}