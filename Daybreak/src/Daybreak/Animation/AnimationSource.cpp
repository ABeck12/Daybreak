#include "dbpch.h"

#include "Daybreak/Animation/AnimationSource.h"

namespace Daybreak
{
    void AnimationSource::AddFrame(const KeyFrame& frame)
    {
        m_KeyFrames.emplace_back(frame);
    }

    void AnimationSource::UpdateSource(DeltaTime dt)
    {
        m_PlaybackTime += dt;
        if (m_CurrentFrame < GetMaxFrames()-1)
        {
            if (m_PlaybackTime >= m_KeyFrames[m_CurrentFrame + 1].StartTime)
                m_CurrentFrame += 1;
        }
        else if (m_CurrentFrame == GetMaxFrames()-1)
        {
            if (m_LoopPlayback)
            {
                m_CurrentFrame = 0;
                m_PlaybackTime = 0.0f;
            }
        }
    }
}