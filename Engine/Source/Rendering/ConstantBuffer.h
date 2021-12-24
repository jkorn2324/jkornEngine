#pragma once

struct ID3D11Buffer;

namespace Engine
{

	enum ConstantBufferFlags
	{
		VERTEX_SHADER = 1 << 1,
		PIXEL_SHADER = 1 << 2,
		HULL_SHADER = 1 << 3,
		COMPUTE_SHADER = 1 << 4

	};

	class ConstantBuffer
	{
	public:
		ConstantBuffer(const ConstantBuffer& buf) = delete;

		explicit ConstantBuffer(const void* buffer, std::size_t stride);
		~ConstantBuffer();

		void Update(const void* buffer, std::size_t stride);

	private:
		void Bind(const std::uint32_t& slot, int flags) const;

	private:
		ID3D11Buffer* m_constantBuffer;

		friend class GraphicsRenderer;
	};
}