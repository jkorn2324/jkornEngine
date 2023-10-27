#pragma once

#include "VertexArray.h"
#include <vector>

struct ID3D11Buffer;

namespace Engine
{
	class DirectX11IndexBuffer;
	class DirectX11VertexBuffer;

	class DirectX11VertexArray : public VertexArray
	{
	public:
		explicit DirectX11VertexArray();
		~DirectX11VertexArray();

		void SetVertexBuffers(const std::vector<std::shared_ptr<VertexBuffer>>& buffers) override;
		void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		bool GetVertexBuffer(std::shared_ptr<VertexBuffer>& buf, uint32_t index) const override;

		size_t GetNumVertexBuffers() const override { return m_vertexBuffers.size(); }
		void Bind() const override;

		void ClearVertexBuffers() override;

	private:
		std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
	};
}