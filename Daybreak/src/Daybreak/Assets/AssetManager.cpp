#include "dbpch.h"

#include "Daybreak/Assets/AssetManager.h"

#include "Daybreak/Renderer/Texture.h"

namespace Daybreak
{
	std::unordered_map<std::filesystem::path, std::variant<Ref<Texture2D>, Ref<Animation>, Ref<AnimationController>>> AssetManager::s_AssetMap;
	std::unordered_map<std::filesystem::path, AssetType> AssetManager::s_AssetTypeMap;

	bool AssetManager::HasAssetRef(const std::filesystem::path& localFilepath)
	{
		return s_AssetMap.find(localFilepath) != s_AssetMap.end();
	}

	bool AssetManager::HasAssetRef(const Ref<Texture2D>& texture)
	{
		for (const auto kv : s_AssetTypeMap)
		{
			if (kv.second == AssetType::Texture2D && std::get<Ref<Texture2D>>(s_AssetMap[kv.first]) == texture)
			{
				return true;
			}
		}
		return false;
	}

	bool AssetManager::HasAssetRef(const Ref<Animation>& anim)
	{
		for (const auto kv : s_AssetTypeMap)
		{
			if (kv.second == AssetType::Animation && std::get<Ref<Animation>>(s_AssetMap[kv.first]) == anim)
			{
				return true;
			}
		}
		return false;
	}

	bool AssetManager::HasAssetRef(const Ref<AnimationController>& controller)
	{
		for (const auto kv : s_AssetTypeMap)
		{
			if (kv.second == AssetType::AnimationController && std::get<Ref<AnimationController>>(s_AssetMap[kv.first]) == controller)
			{
				return true;
			}
		}
		return false;
	}


	template<>
	void AssetManager::AddAssetRef(const Ref<Texture2D> asset, const std::filesystem::path& localFilepath)
	{
		auto it = s_AssetMap.find(localFilepath);
		if (it != s_AssetMap.end())
		{
			DB_CORE_WARN("AsserManager already has asset at {}", localFilepath);
		}
		s_AssetTypeMap[localFilepath] = AssetType::Texture2D;
		s_AssetMap[localFilepath] = asset;
	}

	template<>
	void AssetManager::AddAssetRef(const Ref<Animation> asset, const std::filesystem::path& localFilepath)
	{
		auto it = s_AssetMap.find(localFilepath);
		if (it != s_AssetMap.end())
		{
			DB_CORE_WARN("AsserManager already has asset at {}", localFilepath);
		}
		s_AssetTypeMap[localFilepath] = AssetType::Animation;
		s_AssetMap[localFilepath] = asset;
	}

	template<>
	void AssetManager::AddAssetRef(const Ref<AnimationController> asset, const std::filesystem::path& localFilepath)
	{
		auto it = s_AssetMap.find(localFilepath);
		if (it != s_AssetMap.end())
		{
			DB_CORE_WARN("AsserManager already has asset at {}", localFilepath);
		}
		s_AssetTypeMap[localFilepath] = AssetType::AnimationController;
		s_AssetMap[localFilepath] = asset;
	}

	template<>
	const std::filesystem::path AssetManager::GetFilepathOfRef(const Ref<Texture2D> asset)
	{
		// for (const auto kv : s_AssetMap)
		// {
		// 	if (std::get<Ref<Texture2D>>(kv.second) == asset)
		// 	{
		// 		return kv.first;
		// 	}
		// }
		for (const auto kv : s_AssetTypeMap)
		{
			if (kv.second == AssetType::Texture2D && std::get<Ref<Texture2D>>(s_AssetMap[kv.first]) == asset)
			{
				return kv.first;
			}
		}
		return "unknown.sprite";
	}

	template<>
	const std::filesystem::path AssetManager::GetFilepathOfRef(const Ref<Animation> asset)
	{
		// for (const auto kv : s_AssetMap)
		// {
		// 	if (std::get<Ref<Animation>>(kv.second) == asset)
		// 	{
		// 		return kv.first;
		// 	}
		// }
		for (const auto kv : s_AssetTypeMap)
		{
			if (kv.second == AssetType::Animation && std::get<Ref<Animation>>(s_AssetMap[kv.first]) == asset)
			{
				return kv.first;
			}
		}
		return "unknown.anim";
	}

	template<>
	const std::filesystem::path AssetManager::GetFilepathOfRef(const Ref<AnimationController> asset)
	{
		// for (const auto kv : s_AssetMap)
		// {
		// 	if (std::get<Ref<AnimationController>>(kv.second) == asset)
		// 	{
		// 		return kv.first;
		// 	}
		// }
		for (const auto kv : s_AssetTypeMap)
		{
			if (kv.second == AssetType::AnimationController && std::get<Ref<AnimationController>>(s_AssetMap[kv.first]) == asset)
			{
				return kv.first;
			}
		}
		return "unknown.controller";
	}


}