#include "dbpch.h"

#include "Daybreak/Renderer/Texture.h"

#include "Platform/OpenGL/OpenGLTexture.h"

namespace Daybreak
{
	Ref<Texture> Texture::Create(const TextureSpecifications& textureSpecs, const std::string& filepath)
	{
		return CreateRef <OpenGLTexture>(textureSpecs, filepath);
	}
}