#include "dbpch.h"

#include "Daybreak/AssetSource/AnimationSource.h"

namespace Daybreak
{
	// void AnimationSource::AddKeyFrame(const KeyFrame& frame)
	// {
	// 	m_KeyFrames.emplace_back(frame);
	// }

	void AnimationSource::AddKeyFrame(const Ref<SubTexture2D>& subTexture, const uint32_t& numberFrames, const AnimationAction& action)
	{
		KeyFrame frame;
		frame.Sprite = subTexture;
		frame.Duration = numberFrames * (1.0f / 60.0f);
		frame.Action = action;
		m_KeyFrames.emplace_back(frame);
	}

	void AnimationSource::UpdateSource(DeltaTime dt)
	{
		if (m_CurrentKeyFrame < GetMaxKeyFrames())
		{
			m_DisplayTime += dt;
			if (m_DisplayTime >= m_KeyFrames[m_CurrentKeyFrame].Duration)
			{
				IncrementKeyFrame();
			}
		}
		else if (m_CurrentKeyFrame == GetMaxKeyFrames() && m_LoopPlayback)
		{
			ResetSource();
		}
		// DB_LOG((uint32_t)(m_PlaybackTime / frameDuration));
		// uint32_t currentFrame = (uint32_t)floor(m_PlaybackTime / frameDuration);
		// if (currentFrame != m_CurrentFrame)
		// {
		// 	// std::vector<AnimationAction>& actions = m_AnimationActions.find(currentFrame);
		// 	DB_LOG(currentFrame);
		// 	m_CurrentFrame = currentFrame;
		// }
	}

	void AnimationSource::IncrementKeyFrame()
	{
		m_DisplayTime -= m_KeyFrames[m_CurrentKeyFrame].Duration;
		m_CurrentKeyFrame += 1;
		if (m_KeyFrames[m_CurrentKeyFrame].Action)
		{
			m_KeyFrames[m_CurrentKeyFrame].Action();	
		}
	}

	void AnimationSource::ResetSource()
	{
		m_CurrentKeyFrame = 0;
		m_DisplayTime = 0.0f;
	}
}