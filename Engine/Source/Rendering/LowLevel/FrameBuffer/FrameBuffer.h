#pragma once

#include <initializer_list>
#include <vector>

#include "FrameBufferSpecification.h"

namespace Engine
{
	class Texture;

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

		virtual Texture* GetDepthTexture() const =0;
		virtual Texture* GetRenderTargetTexture(uint32_t index) const =0;

		size_t GetNumRenderTargets() const { return m_frameBufferSpecification.attachments.colorAttachments.size(); }

	protected:
		FrameBufferSpecification m_frameBufferSpecification;

	public:
		static FrameBuffer* Create(const FrameBufferSpecification& specification);
	};
}
