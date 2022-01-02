#pragma once

struct ID3D11Buffer;

namespace Engine
{

	enum ConstantBufferFlags
	{
		VERTEX_SHADER = 1 << 0,
		PIXEL_SHADER = 1 << 1,
		HULL_SHADER = 1 << 2,
		COMPUTE_SHADER = 1 << 3

	};

	class ConstantBuffer
	{
	public:
		ConstantBuffer(const ConstantBuffer& buf) = delete;
		explicit ConstantBuffer(const void* buffer, std::size_t stride);
		virtual ~ConstantBuffer() { }

		virtual void SetData(const void* buffer, std::size_t stride)=0;

	protected:
		virtual void Bind(const std::uint32_t& slot, int flags) const=0;
		
	public:
		static ConstantBuffer* Create(const void* buffer, std::size_t stride);

		friend class GraphicsRenderer;
	};
}