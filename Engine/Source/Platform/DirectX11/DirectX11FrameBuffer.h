#pragma once

#include "FrameBuffer.h"
#include "DirectX11Texture.h"

#include <d3d11.h>

namespace Engine
{

	struct DirectX11ViewTexture
	{
		DirectX11Texture* texture = nullptr;
		ID3D11View* m_view = nullptr;

		~DirectX11ViewTexture()
		{
			if (m_view != nullptr)
			{
				m_view->Release();
			}
			delete texture;
		}
	};

	class DirectX11FrameBuffer : public FrameBuffer
	{
	public:
		explicit DirectX11FrameBuffer(const FrameBufferSpecification& specification);
		~DirectX11FrameBuffer();

		void Bind() const override;
		void UnBind() const override;

		class Texture* GetTexture(FrameBufferAttachmentType type) const override;

	private:
		void CreateBuffers();

		void CreateViewTexture(DirectX11ViewTexture* viewTexture,
			class DirectX11RenderingAPI* api, const FrameBufferAttachment& attachment);
		void CreateTextureWithShaderResource(class DirectX11RenderingAPI* api, UINT bindFlags, DXGI_FORMAT format,
			UINT quality, ID3D11Texture2D** texture2D, ID3D11ShaderResourceView** shaderResource, DXGI_FORMAT shaderResourceFormat = DXGI_FORMAT_UNKNOWN);
		void CreateTexture(class DirectX11RenderingAPI* api, UINT bindFlags, DXGI_FORMAT format, UINT quality, ID3D11Texture2D** texture2D);

	private:
		DirectX11ViewTexture m_depthTexture;
		DirectX11ViewTexture m_renderTargetTexture;

		ID3D11DepthStencilState* m_depthStencilState;
	};
}