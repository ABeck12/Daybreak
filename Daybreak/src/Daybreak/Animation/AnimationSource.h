#pragma once

#include "dbpch.h"

#include "Daybreak/Renderer/SubTexture.h"
#include "Daybreak/Renderer/Texture.h"
#include "Daybreak/Core/DeltaTime.h"

namespace Daybreak
{
    struct KeyFrame
    {
        Ref<SubTexture2D> Sprite;
        float StartTime = 0.0f;
    };

    class AnimationSource
    {
    public:
        void AddFrame(const KeyFrame& frame);

        KeyFrame GetCurrentKeyFrame() const { return m_KeyFrames[m_CurrentFrame]; }
        uint32_t GetMaxFrames() const { return (uint32_t)m_KeyFrames.size(); }

        void UpdateSource(const DeltaTime dt);

    private:
        std::vector<KeyFrame> m_KeyFrames;
        
        bool m_LoopPlayback = true;
        uint32_t m_CurrentFrame = 0;
        float m_PlaybackTime = 0.0f;
    };
}