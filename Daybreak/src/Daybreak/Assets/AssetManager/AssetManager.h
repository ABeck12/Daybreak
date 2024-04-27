#pragma once

#include "dbpch.h"

#include "Daybreak/Renderer/Texture.h"
#include "Daybreak/Renderer/Shader.h"
#include "Daybreak/Assets/Animation.h"

namespace Daybreak
{
	class AssetManager
	{
	public:
		struct AssetMaps
		{
			std::unordered_map<std::filesystem::path, Ref<Texture2D>> Texture2Ds;
			std::unordered_map<std::filesystem::path, Ref<Shader>> Shaders;
			std::unordered_map<std::filesystem::path, Ref<Animation>> Animations;
			std::unordered_map<std::filesystem::path, Ref<AnimationController>> AnimationControllers;
		};

		static AssetManager* Get() { return s_AssetManager; }
		static void Init();
		static void Shutdown();

		// void Clear();

		const std::filesystem::path& GetAssetDir() const
		{
			DB_CORE_ASSERT(s_AssetManager, "AssetManager singleton does not exist");
			return m_AssetDirectoryPath;
		}

		Ref<Texture2D> LoadTexture2D(const std::filesystem::path& assetPath);
		Ref<Shader> LoadShader(const std::filesystem::path& assetPath);
		Ref<Animation> LoadAnimation(const std::filesystem::path& assetPath);
		Ref<AnimationController> LoadAnimationController(const std::filesystem::path& assetPath);

		bool HasAssetPath(const std::filesystem::path& path);

		void FindAssetsDirectory();

		std::filesystem::path GetTexture2DFilepath(const Ref<Texture2D>& texture);
		std::filesystem::path GetAnimationFilepath(const Ref<Animation>& anim);
		std::filesystem::path GetAnimationControllerFilepath(const Ref<AnimationController>& anim);

		void SerializeTexture2D(const Ref<Texture2D>& texture);
		void SerializeTexture2D(const Ref<Texture2D>& texture, const std::filesystem::path& assetPath);
		void SerializeAnimation(const Ref<Animation>& anim, const std::filesystem::path& assetPath);
		void SerializeAnimationController(const Ref<AnimationController>& controller, const std::filesystem::path& assetPath);

	private:
		Ref<Texture2D> DeserializeTexture2D(const std::filesystem::path& filepath);
		Ref<Animation> DeserializeAnimation(const std::filesystem::path& filepath);
		Ref<AnimationController> DeserializeAnimationController(const std::filesystem::path& filepath);

	private:
		AssetMaps m_Assets;
		std::filesystem::path m_AssetDirectoryPath;

		static AssetManager* s_AssetManager;
	};
}