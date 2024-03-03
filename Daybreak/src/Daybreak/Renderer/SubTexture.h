#pragma once

#include "dbpch.h"

#include "Daybreak/Renderer/Texture.h"

namespace Daybreak
{
    class SubTexture2D
    {
    public:
        SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

        const glm::vec2* GetTextureCoords() const { return m_TexCoords; }
        const Ref<Texture2D> GetTexture() const { return m_Texture; }
        const uint32_t GetWidth() const { return m_Width; }
        const uint32_t GetHeight() const { return m_Height; }

        static Ref<SubTexture2D> Create(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& cellCount = { 1, 1 });
    private:
        Ref<Texture2D> m_Texture;
        glm::vec2 m_TexCoords[4];
        uint32_t m_Width, m_Height;
    };
}