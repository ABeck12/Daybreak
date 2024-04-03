#pragma once

#include "Daybreak/Renderer/Texture.h"

#include "Daybreak/Assets/Animation.h"

namespace Daybreak
{
	// LocalFilepaths are relatiive to the "Sandbox/assets/" directory
	class AssetSerializer
	{
	public:
		AssetSerializer() = delete;

		static void SerializeSprite(const Ref<Texture2D>& texture, const std::filesystem::path& localFilepath);
		static void SerializeAnimation(const Ref<Animation>& anim, const std::filesystem::path& localFilepath);
		static void SerializeAnimationController(const Ref<AnimationController>& controller, const std::filesystem::path& localFilepath);

		static Ref<Texture2D> DeserializeSprite(const std::filesystem::path& localFilepath);
		static Ref<Animation> DeserializeAnimation(const std::filesystem::path& localFilepath);
		static Ref<AnimationController> DeserializeAnimationController(const std::filesystem::path& localFilepath);
	};
}