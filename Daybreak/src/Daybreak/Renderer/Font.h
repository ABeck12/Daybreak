#pragma once

#include <filesystem>

#include "Daybreak/Core/Base.h"
#include "Daybreak/Renderer/Texture.h"

namespace Daybreak
{
	struct MSDFData;

	struct CharacterSet
	{
		uint16_t Min;
		uint16_t Max;
	};

	class Font
	{
	public:
		Font(const std::filesystem::path& font, CharacterSet charSet = { 0x0020, 0x00FF });
		~Font();

		const MSDFData* GetMSDFData() const { return m_Data; }
		Ref<Texture2D> GetAtlasTexture() const { return m_AtlasTexture; }

		const std::string& GetName() const { return m_FontName; }
		const std::filesystem::path& GetFilepath() const { return m_Filepath; }

		// TODO: move into asset manager
		static Ref<Font> GetDefault();

	private:
		std::string m_FontName;
		std::filesystem::path m_Filepath;
		MSDFData* m_Data;
		Ref<Texture2D> m_AtlasTexture;
	};

}