#pragma once

#include <memory>

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
		explicit ConstantBuffer(const void* buffer, size_t stride);
		virtual ~ConstantBuffer() { }

		virtual void SetData(void* buffer, size_t stride)=0;
		void SetData(const void* buffer, size_t stride);

		virtual void Bind(const uint32_t& slot, int flags) const=0;
		
	public:
		static bool Create(ConstantBuffer** outConstantBuffer,
			const void* buffer, size_t stride);
		static bool Create(std::shared_ptr<ConstantBuffer>& constantBuffer,
			const void* buffer, size_t stride);
		
		friend class GraphicsRenderer;
	};
}