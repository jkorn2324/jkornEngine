#pragma once

#include <string>

#include "Source\Vector.h"

namespace Engine
{

	struct MeshVertex
	{
		MathLib::Vector3 point;
		MathLib::Vector3 normal;
		MathLib::Vector2 uv;
		MathLib::Vector4 color = MathLib::Vector4::One;
	};


	class Mesh
	{
	public:
		explicit Mesh();
		~Mesh();

		void SetIndices(const std::uint32_t* indices, std::uint32_t indexCount);
		void SetVertices(const MeshVertex* vertices, std::uint32_t vertexCount);

		MeshVertex* GetVertices() const;
		std::uint32_t GetNumVertices() const;

		std::uint32_t* GetIndices() const;
		std::uint32_t GetNumIndices() const;

		static Mesh* StaticLoad(const std::wstring& path);

	private:
		bool Load(const std::wstring& path);

		bool LoadFBX(const std::wstring& path);

	private:
		MeshVertex* m_vertices;
		std::uint32_t* m_indices;

		std::uint32_t m_vertexCount;
		std::uint32_t m_indexCount;
	};
}