#pragma once

#include "FrameBuffer.h"
#include <d3d11.h>

namespace Engine
{

	class DirectX11FrameBuffer : public FrameBuffer
	{
	public:
		explicit DirectX11FrameBuffer(const FrameBufferSpecification& specification);
		~DirectX11FrameBuffer();

		void Bind() const override;
		void ClearDepthBuffer() override;

	private:
		void CreateBuffers();

	private:
		ID3D11DepthStencilView* m_depthStencilView;
		ID3D11Texture2D* m_depthTexture;
	};
}