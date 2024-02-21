#pragma once

#include "dbpch.h"

namespace Daybreak
{
    // enum class FrameBufferAttachmentTypes
    // {   
    //     None = 0,
    //     RGBA8,
    //     // RedInteger,
    //     Depth,
    //     // Stencil
    // };

    // struct FrameBufferAttachments
    // {
    //     FrameBufferAttachments() = default;
	// 	FrameBufferAttachments(std::initializer_list<FrameBufferAttachmentTypes> attachments)
	// 		: AttachmentTypes(attachments) {}
    //     std::vector<FrameBufferAttachmentTypes> AttachmentTypes;
    // };

    struct FrameBufferSpecifications
    {
        uint32_t Width = 0;
        uint32_t Height = 0;
        
        // FrameBufferAttachments Attachments;

        bool SwapChainTarget = false;
    };

    class FrameBuffer
    {
    public:
        virtual ~FrameBuffer() = default;

        virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

        virtual const FrameBufferSpecifications& GetSpecification() const = 0;
        virtual const uint32_t GetColorAttachmentRendererID() const = 0;
        virtual void Resize(uint32_t width, uint32_t height) = 0;

        static Ref<FrameBuffer> Create(const FrameBufferSpecifications& spec);
    };
}