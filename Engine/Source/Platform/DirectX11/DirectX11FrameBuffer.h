#pragma once

#include "FrameBuffer.h"
#include "DirectX11Texture.h"

#include <d3d11.h>

namespace Engine
{
	class Texture;

	struct DirectX11ViewTexture
	{
		DirectX11Texture* texture = nullptr;
		ID3D11View* m_view = nullptr;

		~DirectX11ViewTexture()
		{
			Deallocate();
		}

		operator bool() const { return m_view != nullptr && texture != nullptr; }

		void Deallocate()
		{
			if (m_view != nullptr)
			{
				m_view->Release();
				m_view = nullptr;
			}
			delete texture;
			texture = nullptr;
		}
	};

	class DirectX11FrameBuffer : public FrameBuffer
	{
	public:
		static const size_t MaxRenderTextures = D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT;

		explicit DirectX11FrameBuffer(const FrameBufferSpecification& specification);
		~DirectX11FrameBuffer();

		void Bind() const override;
		void UnBind() const override;

		Texture* GetDepthTexture() const override;
		Texture* GetRenderTargetTexture(uint32_t index) const override;

		void ReGenerateTextures() override;
		void Resize(uint32_t width, uint32_t height) override;

	private:
		void CreateBuffers();

	private:
		DirectX11ViewTexture m_depthTexture;
		DirectX11ViewTexture m_renderTargetTextures[MaxRenderTextures];
		ID3D11DepthStencilState* m_depthStencilState;
	};
}