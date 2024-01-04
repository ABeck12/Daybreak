#include "dbpch.h"

#include "Platform/OpenGL/OpenGLTexture.h"

#include <glad/glad.h>

#include "stb_image_temp/stb_image.h"

namespace Daybreak
{
	OpenGLTexture::OpenGLTexture(const TextureSpecifications& textureSpecs, const std::string& filepath) :
		m_textureSpecs(textureSpecs), m_filepath(filepath)
	{
		stbi_set_flip_vertically_on_load(1);
		int m_BPP = 0; // SUPER TEMPORARY
		int width = 128;//(int)m_textureSpecs.width;
		int height = 128;//(int)m_textureSpecs.height;
		m_LocalBuffer = stbi_load(filepath.c_str(), &width, &height, &m_BPP, 4);

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
		glBindTexture(GL_TEXTURE_2D, 0);

		if (m_LocalBuffer)
			stbi_image_free(m_LocalBuffer);
		else
		{
			DB_CORE_ERROR("Failed to load texture {}. {}", filepath, stbi_failure_reason());
		}
	}

	OpenGLTexture::~OpenGLTexture()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	const void OpenGLTexture::Bind(const uint32_t& slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	}

	const void OpenGLTexture::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}