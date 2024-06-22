#include "dbpch.h"

#include "Daybreak/Renderer/SubTexture.h"

namespace Daybreak
{
	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
		: m_Texture(texture)
	{
		m_TexCoords[0] = { min.x, min.y };
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = { max.x, max.y };
		m_TexCoords[3] = { min.x, max.y };

		m_Height = (uint32_t)round((float)texture->GetHeight() * (max.y - min.y));
		m_Width = (uint32_t)round((float)texture->GetWidth() * (max.x - min.x));
	}

	Ref<SubTexture2D> SubTexture2D::Create(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& cellCount)
	{
		glm::vec2 min = { (coords.x * cellSize.x) / texture->GetWidth(), (coords.y * cellSize.y) / texture->GetHeight() };
		glm::vec2 max = { ((coords.x + cellCount.x) * cellSize.x) / texture->GetWidth(), ((coords.y + cellCount.y) * cellSize.y) / texture->GetHeight() };
		return CreateRef<SubTexture2D>(texture, min, max);
	}

}