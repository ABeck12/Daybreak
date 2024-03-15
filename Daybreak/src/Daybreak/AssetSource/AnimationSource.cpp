#include "dbpch.h"

#include "Daybreak/AssetSource/AnimationSource.h"

namespace Daybreak
{
	void Animation::AddKeyFrame(const Ref<SubTexture2D>& subTexture, uint32_t numberFrames, const AnimationAction& action)
	{
		KeyFrame frame;
		frame.Sprite = subTexture;
		frame.Duration = numberFrames * (1.0f / 60.0f);
		frame.Action = action;
		m_KeyFrames.emplace_back(frame);
	}

	void Animation::AddActionForKeyFrame(uint32_t frameIndex, const AnimationAction& action)
	{
		DB_CORE_ASSERT(frameIndex <= GetMaxKeyFrames(), "frameIndex is out of bounds. Cant add action to non-existent KeyFrame!");

		m_KeyFrames[frameIndex].Action = action;
	}

	void Animation::Update(DeltaTime dt)
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
			Reset();
		}
	}

	void Animation::IncrementKeyFrame()
	{
		m_DisplayTime -= m_KeyFrames[m_CurrentKeyFrame].Duration;
		m_CurrentKeyFrame += 1;
		if (m_KeyFrames[m_CurrentKeyFrame].Action)
		{
			m_KeyFrames[m_CurrentKeyFrame].Action();
		}
	}

	void Animation::Reset()
	{
		m_CurrentKeyFrame = 0;
		m_DisplayTime = 0.0f;
	}

	void AnimationController::AddAnimation(const std::string& name, const Ref<Animation>& animation)
	{
		if (m_AnimationMap.find(name) != m_AnimationMap.end())
		{
			DB_CORE_ASSERT(false, "Controller already has animation {}", name);
		}
		m_AnimationMap[name] = animation;
	}

	void AnimationController::RemoveAnimation(const std::string& name)
	{
		if (m_AnimationMap.find(name) == m_AnimationMap.end())
		{
			DB_CORE_ASSERT(false, "Controller does not have animation {} to remove", name);
		}
		m_AnimationMap.erase(name);
	}
}