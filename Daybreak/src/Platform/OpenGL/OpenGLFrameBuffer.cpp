#include "Daybreak/Core/Log.h"
#include "dbpch.h"

#include "Platform/OpenGL/OpenGLFrameBuffer.h"

#include "glad/glad.h"

namespace Daybreak
{
	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecifications& spec)
		: m_Specification(spec)
	{
		Remake();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
	}

	void OpenGLFrameBuffer::Remake()
	{
		// #define CHECK_ERROR                              \
		// 	{                                               \
		// 		GLenum err;                                 \
		// 		while ((err = glGetError()) != GL_NO_ERROR) \
		// 		{                                           \
		// 			DB_CORE_ERROR((int)err);                \
		// 		}                                           \
		// 	}
		glGenFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		int colorAttachments = 0;
		for (FrameBufferAttachmentTypes type : m_Specification.AttachmentTypes)
		{
			switch (type)
			{
				case FrameBufferAttachmentTypes::RGBA:
					uint32_t rgbaID;
					glCreateTextures(GL_TEXTURE_2D, 1, &rgbaID);
					glBindTexture(GL_TEXTURE_2D, rgbaID);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorAttachments, GL_TEXTURE_2D, rgbaID, 0);
					m_AttachmentIDs.emplace_back(rgbaID);
					m_AttachmentEnumsValues.emplace_back(GL_COLOR_ATTACHMENT0 + colorAttachments);
					m_ColorBuffers.emplace_back(GL_COLOR_ATTACHMENT0 + colorAttachments);
					colorAttachments++;
					break;

				case FrameBufferAttachmentTypes::RGBA32F:
					uint32_t rgba32FID;
					glCreateTextures(GL_TEXTURE_2D, 1, &rgba32FID);
					glBindTexture(GL_TEXTURE_2D, rgba32FID);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_FLOAT, nullptr);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorAttachments, GL_TEXTURE_2D, rgba32FID, 0);
					m_AttachmentIDs.emplace_back(rgba32FID);
					m_AttachmentEnumsValues.emplace_back(GL_COLOR_ATTACHMENT0 + colorAttachments);
					m_ColorBuffers.emplace_back(GL_COLOR_ATTACHMENT0 + colorAttachments);
					colorAttachments++;

				case FrameBufferAttachmentTypes::Depth:
					uint32_t depthID;
					glCreateTextures(GL_TEXTURE_2D, 1, &depthID);
					glBindTexture(GL_TEXTURE_2D, depthID);
					glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthID, 0);
					m_AttachmentIDs.emplace_back(depthID);
					m_AttachmentEnumsValues.emplace_back(GL_DEPTH_STENCIL_ATTACHMENT);
					break;

				case FrameBufferAttachmentTypes::RedInteger:
					uint32_t redIntID;
					glCreateTextures(GL_TEXTURE_2D, 1, &redIntID);
					glBindTexture(GL_TEXTURE_2D, redIntID);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, m_Specification.Width, m_Specification.Height, 0, GL_RED_INTEGER, GL_INT, nullptr);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorAttachments, GL_TEXTURE_2D, redIntID, 0);
					m_AttachmentIDs.emplace_back(redIntID);
					m_AttachmentEnumsValues.emplace_back(GL_COLOR_ATTACHMENT0 + colorAttachments);
					m_ColorBuffers.emplace_back(GL_COLOR_ATTACHMENT0 + colorAttachments);
					colorAttachments++;
					break;

				case FrameBufferAttachmentTypes::Stencil:
					break;
			}
		}

		DB_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "FrameBuffer is incomplete");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);

		glDrawBuffers((int)m_ColorBuffers.size(), &m_ColorBuffers[0]);
	}

	void OpenGLFrameBuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		m_AttachmentIDs.clear();
		m_AttachmentEnumsValues.clear();
		m_Specification.Height = height;
		m_Specification.Width = width;
		Remake();
	}

	void OpenGLFrameBuffer::BindAttachmentAsTexture(uint32_t attachmentIndex, uint32_t textureSlot) const
	{
		glActiveTexture(GL_TEXTURE0 + textureSlot);
		glBindTexture(GL_TEXTURE_2D, GetAttachmentRendererID(attachmentIndex));
	}

	int OpenGLFrameBuffer::ReadPixel1I(uint32_t attachmentIndex, int x, int y) const
	{
		DB_CORE_ASSERT(attachmentIndex < m_AttachmentEnumsValues.size(), "Unknown attachment index {}", attachmentIndex);

		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}

	void OpenGLFrameBuffer::Blit(const Ref<FrameBuffer>& target, uint32_t sourceAttachment, uint32_t targetAttachment) const
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, GetRendererID());
		glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + sourceAttachment,
							   GL_TEXTURE_2D, GetAttachmentRendererID(), 0);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, target->GetRendererID());
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + targetAttachment,
							   GL_TEXTURE_2D, target->GetAttachmentRendererID(), 0);


		glBlitFramebuffer(0, 0, GetSpecification().Width, GetSpecification().Height,
						  0, 0, target->GetSpecification().Width, target->GetSpecification().Height,
						  GL_COLOR_BUFFER_BIT, GL_LINEAR);
	}
}
