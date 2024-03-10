#pragma once

#include "Daybreak/Renderer/FrameBuffer.h"

namespace Daybreak
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecifications& spec);
		virtual ~OpenGLFrameBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const FrameBufferSpecifications& GetSpecification() const override { return m_Specification; }
		virtual const uint32_t GetAttachmentRendererID(uint32_t index = 0) const override { return m_AttachmentIDs[index]; }
		virtual void Resize(uint32_t width, uint32_t height) override;

	private:
		void Remake();

	private:
		FrameBufferSpecifications m_Specification;
		uint32_t m_RendererID;

		std::vector<uint32_t> m_AttachmentIDs;
	};
}