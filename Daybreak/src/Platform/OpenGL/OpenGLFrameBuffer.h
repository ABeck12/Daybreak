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
        virtual const uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }
        virtual void Resize(uint32_t width, uint32_t height) override;

    private:
        void Remake();
    private:
        FrameBufferSpecifications m_Specification;
        uint32_t m_RendererID;

        uint32_t m_DepthAttachment;
        uint32_t m_ColorAttachment;
    };
}