#include "dbpch.h"

#include "Daybreak/Renderer/Texture.h"

#include "Platform/OpenGL/OpenGLTexture.h"

namespace Daybreak
{
	Ref<Texture2D> Texture2D::Create(const TextureSpecifications& textureSpecs, const std::filesystem::path& filepath)
	{
		return CreateRef<OpenGLTexture2D>(textureSpecs, filepath);
	}

	Ref<Texture2D> Texture2D::Create(const TextureSpecifications& textureSpecs)
	{
		return CreateRef<OpenGLTexture2D>(textureSpecs);
	}
}