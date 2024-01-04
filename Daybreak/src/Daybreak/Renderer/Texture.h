#pragma once

namespace Daybreak
{
	enum class ImageFormat
	{
		None = 0,
		R8, RGB8, RGBA8
	};

	struct TextureSpecifications //FIXME: Change the hardcoding ASAP
	{
		//TextureSpecifications() {};
		//TextureSpecifications(uint32_t width, uint32_t height, ImageFormat format)
		uint32_t width = 128;
		uint32_t height = 128;
		ImageFormat format = ImageFormat::RGB8;
	};

	class Texture
	{
	public:
		~Texture() = default;
		static Ref<Texture> Create(const TextureSpecifications& textureSpecs, const std::string& filepath);

		virtual const std::string& GetFilepath() const = 0;
		virtual const TextureSpecifications& GetTexutreSpecifications() const = 0;

		virtual const uint32_t GetWidth() const = 0;
		virtual const uint32_t GetHeight() const = 0;
		virtual const uint32_t GetRendererID() const = 0;

		virtual const void Bind(const uint32_t& slot = 0) const = 0;
		virtual const void Unbind() const = 0;
		
	};

	//class Texture2D : public Texture
	//{
	//public:
	//	static Ref<Texture2D> Create(const std::string& name);
	//};
}
