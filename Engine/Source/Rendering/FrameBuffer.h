#pragma once

#include <initializer_list>
#include <vector>

#include <d3d11.h>

namespace Engine
{

	enum FrameBufferAttachmentType
	{
		FRAME_BUFFER_ATTACHMENT_TYPE_NONE,
		
		// Depth Stencil
		TYPE_DEPTH24_STENCIL8,
		DEPTH_STENCIL = TYPE_DEPTH24_STENCIL8,

		// Render Target View
		TYPE_RGB,
		RENDER_TARGET = TYPE_RGB
	};

	struct FrameBufferAttachment
	{
		FrameBufferAttachmentType textureType;
		bool exportAsTexture = false;

		FrameBufferAttachment() = default;
		FrameBufferAttachment(FrameBufferAttachmentType textureType)
			: textureType(textureType), exportAsTexture(textureType == TYPE_RGB) { }
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
		
		const FrameBufferSpecification& GetSpecification() const { return m_frameBufferSpecification; }

		virtual class Texture* GetTexture(FrameBufferAttachmentType type) const=0;

	protected:
		FrameBufferSpecification m_frameBufferSpecification;
		FrameBufferAttachment m_depthStencilSpecification = FrameBufferAttachmentType::FRAME_BUFFER_ATTACHMENT_TYPE_NONE;
		FrameBufferAttachment m_renderTargetSpecification = FrameBufferAttachmentType::FRAME_BUFFER_ATTACHMENT_TYPE_NONE;

	public:
		static FrameBuffer* Create(const FrameBufferSpecification& specification);
	};
}