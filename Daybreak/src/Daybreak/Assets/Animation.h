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

	class Animation
	{
	public:
		void AddKeyFrame(const Ref<SubTexture2D>& subTexture, uint32_t numberFrames, const AnimationAction& action = nullptr);
		void AddKeyFrame(const KeyFrame& frame);

		void AddActionForKeyFrame(uint32_t frameIndex, const AnimationAction& action);

		KeyFrame GetCurrentKeyFrame() const { return m_KeyFrames[m_CurrentKeyFrame]; }
		uint32_t GetMaxKeyFrames() const { return (uint32_t)(m_KeyFrames.size() - 1); }

		void SetLoopPlayback(bool loopPlayback) { m_LoopPlayback = loopPlayback; }


	private:
		void Update(const DeltaTime dt);
		void IncrementKeyFrame();
		void Reset();

	private:
		std::vector<KeyFrame> m_KeyFrames;

		bool m_LoopPlayback = true;
		uint32_t m_CurrentKeyFrame = 0;
		float m_DisplayTime = 0.0f;

		friend class AnimationController;
		friend class AssetSerializer;
	};

	class AnimationController
	{
	public:
		void AddAnimation(const std::string& name, const Ref<Animation>& animation);
		void RemoveAnimation(const std::string& name);

		void Update(const DeltaTime dt) { m_ActiveAnimation->Update(dt); }

		void ChangeAnimation(const std::string& name) { m_ActiveAnimation = m_AnimationMap[name]; }
		const Ref<Animation>& GetActiveAnimation() const { return m_ActiveAnimation; }
		void SetStartupAnimation(const std::string& name)
		{
			m_StartupAnimation = name;
			m_ActiveAnimation = m_AnimationMap[name];
		}

	private:
		std::unordered_map<std::string, Ref<Animation>> m_AnimationMap;
		Ref<Animation> m_ActiveAnimation;

		std::string m_StartupAnimation;

		friend class AssetSerializer;
	};
}