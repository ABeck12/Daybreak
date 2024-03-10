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
		glGenFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		for (FrameBufferAttachmentTypes type : m_Specification.AttachmentTypes)
		{
			switch (type)
			{
				case FrameBufferAttachmentTypes::RGBA8:
					uint32_t rgbaID;
					glCreateTextures(GL_TEXTURE_2D, 1, &rgbaID);
					glBindTexture(GL_TEXTURE_2D, rgbaID);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rgbaID, 0);
					m_AttachmentIDs.emplace_back(rgbaID);
					break;

				case FrameBufferAttachmentTypes::Depth:
					uint32_t depthID;
					glCreateTextures(GL_TEXTURE_2D, 1, &depthID);
					glBindTexture(GL_TEXTURE_2D, depthID);
					glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthID, 0);
					break;

				case FrameBufferAttachmentTypes::RedInteger:
					break;

				case FrameBufferAttachmentTypes::Stencil:
					break;
			}
		}

		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


		DB_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "FrameBuffer is incomplete");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		// glViewport(0, 0, m_Specification.Width, m_Specification.Height);
	}

	void OpenGLFrameBuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		m_Specification.Height = height;
		m_Specification.Width = width;
		Remake();
	}
}