#include "dbpch.h"

#include "Daybreak/Assets/Animation.h"

namespace Daybreak
{
	Animation::Animation(const std::string& name)
		: m_Name(name)
	{
	}

	void Animation::AddKeyFrame(const Ref<SubTexture2D>& subTexture, uint32_t numberFrames, const AnimationAction& action)
	{
		KeyFrame frame;
		frame.Sprite = subTexture;
		frame.Duration = numberFrames * (1.0f / 60.0f);
		frame.Action = action;
		m_KeyFrames.emplace_back(frame);
	}

	void Animation::AddKeyFrame(const KeyFrame& frame)
	{
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

	void AnimationController::AddAnimation(const Ref<Animation>& animation)
	{
		std::string name = animation->GetName();
		DB_CORE_ASSERT(!HasAnimation(name), "Controller already has animation {}", name);
		m_AnimationMap[name] = animation;
	}

	void AnimationController::ChangeAnimation(const std::string& name)
	{
		DB_CORE_ASSERT(HasAnimation(name), "Controller does not have animation {}", name);
		m_ActiveAnimation = m_AnimationMap[name];
	}

	void AnimationController::RemoveAnimation(const std::string& name)
	{
		DB_CORE_ASSERT(m_AnimationMap.find(name) != m_AnimationMap.end(), "Controller does not have animation {} to remove", name);
		m_AnimationMap.erase(name);
	}

	bool AnimationController::HasAnimation(const std::string& name) const
	{
		return m_AnimationMap.find(name) != m_AnimationMap.end();
	}

	void AnimationController::SetStartupAnimation(const std::string& name)
	{
		DB_CORE_ASSERT(HasAnimation(name), "Controller does not have animation {}", name);
		m_StartupAnimation = name;
		m_ActiveAnimation = m_AnimationMap[name];
	}
}