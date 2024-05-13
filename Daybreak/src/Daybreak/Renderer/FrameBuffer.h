#pragma once

#include "dbpch.h"

namespace Daybreak
{
	enum class FrameBufferAttachmentTypes
	{
		RGBA,
		RedInteger,
		Depth,
		Stencil
	};

	struct FrameBufferSpecifications
	{
		uint32_t Width = 0;
		uint32_t Height = 0;

		std::vector<FrameBufferAttachmentTypes> AttachmentTypes { FrameBufferAttachmentTypes::RGBA, FrameBufferAttachmentTypes::Depth };

		// bool SwapChainTarget = false;
	};

	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual const void BindAttachmentAsTexture(uint32_t attachmentIndex = 0, uint32_t textureSlot = 0) const = 0;

		virtual const FrameBufferSpecifications& GetSpecification() const = 0;
		virtual const uint32_t GetAttachmentRendererID(uint32_t index = 0) const = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		static Ref<FrameBuffer> Create(const FrameBufferSpecifications& spec);
	};
}