#pragma once

#include <initializer_list>
#include <vector>

#include <d3d11.h>

namespace Engine
{

	enum FrameBufferAttachmentType
	{
		TYPE_NONE,
		
		// Depth Stencil
		TYPE_DEPTH24_STENCIL8,
		DEPTH_STENCIL = TYPE_DEPTH24_STENCIL8
	};

	struct FrameBufferAttachment
	{
		FrameBufferAttachmentType textureType;

		FrameBufferAttachment() = default;
		FrameBufferAttachment(FrameBufferAttachmentType textureType)
			: textureType(textureType) { }
	};

	struct FrameBufferSpecificationAttachmentList
	{
		std::vector<FrameBufferAttachment> attachments;

		FrameBufferSpecificationAttachmentList() = default;
		FrameBufferSpecificationAttachmentList(const std::initializer_list<FrameBufferAttachment>& attachments)
			: attachments(attachments) { }

		std::vector<FrameBufferAttachment>::const_iterator begin() const
		{
			return attachments.begin();
		}

		std::vector<FrameBufferAttachment>::const_iterator end() const
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

		// Rasterization

		FrameBufferSpecificationAttachmentList attachments;

		FrameBufferSpecification() = default;
		FrameBufferSpecification(const FrameBufferSpecificationAttachmentList& attachments)
			: attachments(attachments) { }
	};

	class FrameBuffer
	{

	public:
		explicit FrameBuffer(const FrameBufferSpecification& specification);
		virtual ~FrameBuffer() { }

		virtual void Bind() const=0;
		virtual void ClearDepthBuffer()=0;

	protected:
		FrameBufferSpecification m_frameBufferSpecification;
		FrameBufferAttachment m_depthStencilSpecification = FrameBufferAttachmentType::TYPE_NONE;

	public:
		static FrameBuffer* Create(const FrameBufferSpecification& specification);
	};
}