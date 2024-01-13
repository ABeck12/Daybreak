#include "dbpch.h"

#include "Platform/OpenGL/OpenGLTexture.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Daybreak
{
	OpenGLTexture2D::OpenGLTexture2D(const TextureSpecifications& textureSpecs) : 
		m_Specification(textureSpecs), m_Width(textureSpecs.Width), m_Height(textureSpecs.Height)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, GL_RGBA8, m_Width, m_Height);


		//int glFilterType;
		//if (m_Specification.Filter == TextureFilterType::Bilinear)
		//	glFilterType = GL_LINEAR;
		//else if (m_Specification.Filter == TextureFilterType::Point)
		//	glFilterType = GL_NEAREST;
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glFilterType);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glFilterType);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	}

	OpenGLTexture2D::OpenGLTexture2D(const TextureSpecifications& textureSpecs, const std::string& filepath) :
		m_Specification(textureSpecs), m_Width(textureSpecs.Width), m_Height(textureSpecs.Height)
	{
		stbi_set_flip_vertically_on_load(1);
		int m_BPP; // SUPER TEMPORARY
		int width;
		int height;
		m_LocalBuffer = stbi_load(filepath.c_str(), &width, &height, &m_BPP, 4);
		m_Specification.Height = height;
		m_Specification.Width = width;

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		int glFilterType;
		if (textureSpecs.Filter == TextureFilterType::Bilinear)
			glFilterType = GL_LINEAR;
		else if (textureSpecs.Filter == TextureFilterType::Point)
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

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		auto m_DataFormat = GL_RGBA; //Temp
		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		DB_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
}