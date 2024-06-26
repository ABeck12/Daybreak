#pragma once

#include "dbpch.h"

#include "Daybreak/Renderer/Texture.h"

namespace Daybreak
{
	class OpenGLTexture2D final : public Texture2D
	{
	public:
		OpenGLTexture2D(const TextureSpecifications& textureSpecs);
		OpenGLTexture2D(const TextureSpecifications& textureSpecs, const std::filesystem::path& filepath);
		virtual ~OpenGLTexture2D();

		virtual const std::filesystem::path& GetFilepath() const override { return m_Filepath; }
		virtual const TextureSpecifications& GetTexutreSpecifications() const override { return m_Specification; }

		virtual const uint32_t GetWidth() const override { return m_Specification.Width; }
		virtual const uint32_t GetHeight() const override { return m_Specification.Height; }
		virtual const uint32_t GetRendererID() const override { return m_RendererID; }

		virtual const void Bind(const uint32_t& slot = 0) const override;
		virtual const void Unbind() const override;

		virtual const void SetData(void* data, uint32_t size) const override;

		virtual bool operator==(const Texture& other) const override { return m_RendererID == other.GetRendererID(); }

	private:
		uint32_t m_RendererID;
		TextureSpecifications m_Specification;
		std::filesystem::path m_Filepath;
		uint32_t m_Width, m_Height;
		unsigned char* m_LocalBuffer;
	};

}