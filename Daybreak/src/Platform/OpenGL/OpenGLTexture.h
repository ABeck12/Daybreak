#pragma once

#include "dbpch.h"

#include "Daybreak/Renderer/Texture.h"

namespace Daybreak
{
	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(const TextureSpecifications& textureSpecs, const std::string& filepath);
		virtual ~OpenGLTexture();

		virtual const std::string& GetFilepath() const override { return m_filepath; }
		virtual const TextureSpecifications& GetTexutreSpecifications() const override { return m_textureSpecs; }

		virtual const uint32_t GetWidth() const override { return m_textureSpecs.width; }
		virtual const uint32_t GetHeight() const override { return m_textureSpecs.height; }
		virtual const uint32_t GetRendererID() const override { return m_RendererID; }

		virtual const void Bind(const uint32_t& slot = 0) const override;
		virtual const void Unbind() const override;
	private:
		uint32_t m_RendererID;
		TextureSpecifications m_textureSpecs;
		std::string m_filepath;
		unsigned char* m_LocalBuffer;
	};

	//class OpenGLTexture2D : public Texture2D
	//{
	//public:
	//private:
	//	uint32_t m_RendererID;
	//};
}