#include "dbpch.h"

#include "Platform/OpenGL/OpenGLTexture.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Daybreak
{
	OpenGLTexture2D::OpenGLTexture2D(const TextureSpecifications& textureSpecs, const std::string& filepath) :
		m_textureSpecs(textureSpecs), m_filepath(filepath)
	{
		stbi_set_flip_vertically_on_load(1);
		int m_BPP; // SUPER TEMPORARY
		int width;
		int height;
		m_LocalBuffer = stbi_load(filepath.c_str(), &width, &height, &m_BPP, 4);
		m_textureSpecs.height = height;
		m_textureSpecs.width = width;

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		int glFilterType;
		if (textureSpecs.filter == TextureFilterType::Bilinear)
			glFilterType = GL_LINEAR;
		else if (textureSpecs.filter == TextureFilterType::Point)
			glFilterType = GL_NEAREST;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glFilterType);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glFilterType);
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

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	const void OpenGLTexture2D::Bind(const uint32_t& slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	}

	const void OpenGLTexture2D::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}