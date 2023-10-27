#pragma once

#include <initializer_list>
#include <vector>

namespace Engine
{

	enum FrameBufferAttachmentType
	{
		FRAME_BUFFER_ATTACHMENT_TYPE_NONE,
		
		// Depth Stencil
		TYPE_DEPTH24_STENCIL8,
		DEPTH_STENCIL = TYPE_DEPTH24_STENCIL8,

		// Render Target View
		TYPE_RGBA_32,
		RENDER_TARGET = TYPE_RGBA_32,
		// Render Target View for Entity ID
		TYPE_INT,
		ENTITY_VIEW_ID = TYPE_INT
	};

	struct FrameBufferAttachment
	{
		FrameBufferAttachmentType textureType;
		bool exportAsTexture = false;

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
		uint32_t width = 0;
		uint32_t height = 0;

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
		virtual void UnBind() const =0;
		
		virtual void Resize(uint32_t width, uint32_t height);
		virtual void ReGenerateTextures() =0;

		const FrameBufferSpecification& GetSpecification() const { return m_frameBufferSpecification; }

		virtual class Texture* GetDepthTexture() const =0;
		virtual class Texture* GetRenderTargetTexture(uint32_t index) const =0;

		size_t GetNumRenderTargets() const { return m_renderTargetSpecifications.size(); }

	protected:
		FrameBufferSpecification m_frameBufferSpecification;
		std::vector<FrameBufferAttachment> m_renderTargetSpecifications;
		FrameBufferAttachment m_depthStencilSpecification = FrameBufferAttachmentType::FRAME_BUFFER_ATTACHMENT_TYPE_NONE;

	public:
		static FrameBuffer* Create(const FrameBufferSpecification& specification);
	};
}
