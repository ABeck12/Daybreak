#pragma once

#include <stdint.h>
#include <filesystem>

#include "Daybreak/Core/Base.h"

namespace Daybreak
{
	enum class ImageFormat
	{
		None = 0,
		RGB,
		RGBA // CHANGE THIS OFF OF 8
	};

	enum class TextureFilterType
	{
		Bilinear,
		Point
	};

	struct TextureSpecifications
	{
		uint32_t Width = 1;
		uint32_t Height = 1;
		ImageFormat Format = ImageFormat::RGBA;
		TextureFilterType Filter = TextureFilterType::Point;
		uint32_t PixelsPerUnit = 100; // TODO: Move out of this class
	};

	class Texture
	{
	public:
		~Texture() = default;

		virtual const std::filesystem::path& GetFilepath() const = 0;
		virtual const TextureSpecifications& GetTexutreSpecifications() const = 0;

		virtual const uint32_t GetWidth() const = 0;
		virtual const uint32_t GetHeight() const = 0;
		virtual const uint32_t GetRendererID() const = 0;

		virtual const void Bind(const uint32_t& slot = 0) const = 0;
		virtual const void Unbind() const = 0;

		virtual const void SetData(void* data, uint32_t size) const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const TextureSpecifications& specification);
		static Ref<Texture2D> Create(const TextureSpecifications& textureSpecs, const std::filesystem::path& filepath);
	};
}
