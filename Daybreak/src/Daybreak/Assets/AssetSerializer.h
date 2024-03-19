#pragma once

#include "Daybreak/Renderer/Texture.h"

#include "Daybreak/Assets/Animation.h"

namespace Daybreak
{
	// LocalFilepaths are relatiive to the "Sandbox/assets/" directory
	class AssetSerializer
	{
	public:
		static void SerializeSprite(const Ref<Texture2D>& texture, const std::string& localFilepath);
		static void SerializeAnimation(const Ref<Animation>& anim, const std::string& localFilepath);
		static void SerializeAnimationController(const Ref<AnimationController>& controller, const std::string& localFilepath);

		static Ref<Texture2D> DeserializeSprite(const std::string& localFilepath);
		static Ref<Animation> DeserializeAnimation(const std::string& localFilepath);
		static Ref<AnimationController> DeserializeAnimationController(const std::string& localFilepath);
	};
}