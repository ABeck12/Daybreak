#pragma once

#include "dbpch.h"

#include "Daybreak/Renderer/SubTexture.h"
#include "Daybreak/Renderer/Texture.h"
#include "Daybreak/Core/DeltaTime.h"

namespace Daybreak
{
	// Rework to use DB_BIND_EVENT_FN
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
		Animation(const std::string& name);

		void AddKeyFrame(const Ref<SubTexture2D>& subTexture, uint32_t numberFrames, const AnimationAction& action = nullptr);
		void AddKeyFrame(const KeyFrame& frame);

		void AddActionForKeyFrame(uint32_t frameIndex, const AnimationAction& action);

		const KeyFrame& GetCurrentKeyFrame() const { return m_KeyFrames[m_CurrentKeyFrame]; }
		uint32_t GetMaxKeyFrames() const { return (uint32_t)(m_KeyFrames.size() - 1); }
		const std::vector<KeyFrame>& GetKeyFrames() const { return m_KeyFrames; }

		const std::string& GetName() const { return m_Name; }

		void SetLoopPlayback(bool loopPlayback) { m_LoopPlayback = loopPlayback; }


	private:
		void Update(const DeltaTime dt);
		void IncrementKeyFrame();
		void Reset();

	private:
		std::vector<KeyFrame> m_KeyFrames;
		std::string m_Name;

		bool m_LoopPlayback = true;
		uint32_t m_CurrentKeyFrame = 0;
		float m_DisplayTime = 0.0f;

		friend class AssetManager;
		friend class AnimationController; // Allows AnimationController to update Animation
	};

	class AnimationController
	{
	public:
		void AddAnimation(const Ref<Animation>& animation);
		void RemoveAnimation(const std::string& name);
		bool HasAnimation(const std::string& name) const;

		void Update(const DeltaTime dt) { m_ActiveAnimation->Update(dt); }

		void ChangeAnimation(const std::string& name);
		const Ref<Animation>& GetActiveAnimation() const { return m_ActiveAnimation; }
		void SetStartupAnimation(const std::string& name);

	private:
		std::unordered_map<std::string, Ref<Animation>> m_AnimationMap;
		Ref<Animation> m_ActiveAnimation;

		std::string m_StartupAnimation;

		friend class AssetManager;
	};
}