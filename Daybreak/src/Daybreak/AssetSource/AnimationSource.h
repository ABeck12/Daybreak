#pragma once

#include "dbpch.h"

#include "Daybreak/Renderer/SubTexture.h"
#include "Daybreak/Renderer/Texture.h"
#include "Daybreak/Core/DeltaTime.h"

namespace Daybreak
{
	using AnimationAction = std::function<void(void)>;

	struct KeyFrame
	{
		Ref<SubTexture2D> Sprite;
		float Duration = 0.0f;
		AnimationAction Action;
	};

	class AnimationSource
	{
	public:
		void AddKeyFrame(const Ref<SubTexture2D>& subTexture, const uint32_t& numberFrames, const AnimationAction& action = nullptr);

		void AddAction(const uint32_t& startingFrame);

		KeyFrame GetCurrentKeyFrame() const { return m_KeyFrames[m_CurrentKeyFrame]; }
		uint32_t GetMaxKeyFrames() const { return (uint32_t)(m_KeyFrames.size()-1); }

		void SetLoopPlayback(bool loopPlayback) { m_LoopPlayback = loopPlayback; }

		void UpdateSource(const DeltaTime dt);

	private:
		void IncrementKeyFrame();
		void ResetSource();
		
	private:
		std::vector<KeyFrame> m_KeyFrames;

		bool m_LoopPlayback = true;
		uint32_t m_CurrentKeyFrame = 0;
		float m_DisplayTime = 0.0f;
	};
}