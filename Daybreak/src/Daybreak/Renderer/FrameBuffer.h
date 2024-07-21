#pragma once

#include <stdint.h>
#include <vector>
#include <glm/glm.hpp>

#include "Daybreak/Core/Base.h"

namespace Daybreak
{
	enum class FrameBufferAttachmentTypes
	{
		RGBA,
		RGBA32F,
		RedInteger,
		Depth,
		Stencil
	};

	struct FrameBufferSpecifications
	{
		uint32_t Width = 0;
		uint32_t Height = 0;

		std::vector<FrameBufferAttachmentTypes> AttachmentTypes { FrameBufferAttachmentTypes::RGBA, FrameBufferAttachmentTypes::Depth };
		glm::vec4 ClearColor = { 0, 0, 0, 1 };
		// bool SwapChainTarget = false;
	};

	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void BindAttachmentAsTexture(uint32_t attachmentIndex = 0, uint32_t textureSlot = 0) const = 0;

		virtual const FrameBufferSpecifications& GetSpecification() const = 0;
		virtual const uint32_t GetAttachmentRendererID(uint32_t index = 0) const = 0;
		virtual const uint32_t GetRendererID() const = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual void Blit(const Ref<FrameBuffer>& target, uint32_t sourceAttachment = 0, uint32_t targetAttachment = 0) const = 0;

		virtual const glm::vec4& GetClearColor() const = 0;

		virtual int ReadPixel1I(uint32_t attachmentIndex, int x, int y) const = 0;

		static Ref<FrameBuffer> Create(const FrameBufferSpecifications& spec);
	};
}
