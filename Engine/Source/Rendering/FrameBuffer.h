#pragma once

#include <initializer_list>
#include <vector>

#include <d3d11.h>

namespace Engine
{

	enum FrameBufferTextureType
	{
		TYPE_NONE,
		
		// Depth Stencil
		TYPE_DEPTH24_STENCIL8,
		DEPTH_STENCIL = TYPE_DEPTH24_STENCIL8
	};

	struct FrameBufferTextureSpecification
	{
		FrameBufferTextureType textureType;

		FrameBufferTextureSpecification() = default;
		FrameBufferTextureSpecification(FrameBufferTextureType textureType)
			: textureType(textureType) { }
	};

	struct FrameBufferSpecificationAttachmentList
	{
		std::vector<FrameBufferTextureSpecification> attachments;

		FrameBufferSpecificationAttachmentList() = default;
		FrameBufferSpecificationAttachmentList(const std::initializer_list<FrameBufferTextureSpecification>& attachments)
			: attachments(attachments) { }

		std::vector<FrameBufferTextureSpecification>::const_iterator begin() const
		{
			return attachments.begin();
		}

		std::vector<FrameBufferTextureSpecification>::const_iterator end() const
		{
			return attachments.end();
		}
	};

	struct FrameBufferSpecification
	{
		// Depth Stencil Parameters.
		std::uint32_t width = 0;
		std::uint32_t height = 0;

		// Sampler Parameters.
		std::uint32_t samples = 1;

		FrameBufferSpecificationAttachmentList attachments;

		FrameBufferSpecification() = default;
		FrameBufferSpecification(const FrameBufferSpecificationAttachmentList& attachments)
			: attachments(attachments) { }
	};

	class FrameBuffer
	{

	public:
		explicit FrameBuffer(const FrameBufferSpecification& specification);
		~FrameBuffer();

		void Bind() const;
		void ClearDepthBuffer();

		// DIRECTX11 SPECIFIC FUNCTIONS
		ID3D11DepthStencilView* GetDepthStencilView() const;

	private:
		void CreateBuffers();

	private:
		// DIRECTX 11 SPECIFIC VARIABLES
		// Depth stencil view for frame buffers.
		ID3D11DepthStencilView* m_depthStencilView;
		ID3D11Texture2D* m_depthTexture;

		FrameBufferSpecification m_frameBufferSpecification;
		FrameBufferTextureSpecification m_depthStencilSpecification = FrameBufferTextureType::TYPE_NONE;
	};
}