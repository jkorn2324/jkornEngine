#pragma once

#include <vector>
#include <initializer_list>

#include "GraphicsFormat.h"

namespace Engine
{

	/**
	 * Get the frame buffer attachment. 
	 */
	template<typename TAttachmentType>
	struct FrameBufferAttachment
	{
		TAttachmentType format;

		FrameBufferAttachment() = default;
		FrameBufferAttachment(const TAttachmentType& format)
			: format(format) { }
	};

	/**
	 * The depth stencil context. 
	 */
	struct FrameBufferDepthStencilContext
	{
		DepthFormat format = DepthFormat_None;
		bool readTexture = false;
	};

	/**
	 * The frame buffer specification attachments. 
	 */
	struct FrameBufferSpecificationAttachmentList
	{
		std::vector<FrameBufferAttachment<ColorFormat>> colorAttachments;
		FrameBufferAttachment<FrameBufferDepthStencilContext> depthStencilAttachment;

		FrameBufferSpecificationAttachmentList() = default;
		FrameBufferSpecificationAttachmentList(const FrameBufferAttachment<FrameBufferDepthStencilContext>& depthStencilAttachment)
			: colorAttachments(), depthStencilAttachment(depthStencilAttachment) { }
		FrameBufferSpecificationAttachmentList(const std::initializer_list<FrameBufferAttachment<ColorFormat>>& attachments)
			: colorAttachments(attachments), depthStencilAttachment() { }

		FrameBufferSpecificationAttachmentList(const FrameBufferAttachment<FrameBufferDepthStencilContext>& depthStencilAttachment, 
			const std::initializer_list<FrameBufferAttachment<ColorFormat>>& attachments)
			: colorAttachments(attachments), depthStencilAttachment(depthStencilAttachment) { }

		std::vector<FrameBufferAttachment<ColorFormat>>::const_iterator begin() const
		{
			return colorAttachments.begin();
		}

		std::vector<FrameBufferAttachment<ColorFormat>>::const_iterator end() const
		{
			return colorAttachments.end();
		}
	};

	/**
	 * The frame buffer specification. 
	 */
	struct FrameBufferSpecification
	{
		FrameBufferSpecificationAttachmentList attachments;
		uint32_t width = 0;
		uint32_t height = 0;

		FrameBufferSpecification() = default;
		FrameBufferSpecification(const FrameBufferSpecificationAttachmentList& attachments)
			: attachments(attachments) { }
	};

}