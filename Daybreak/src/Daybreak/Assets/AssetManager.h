#pragma once

#include "dbpch.h"

#include "Daybreak/Assets/Animation.h"

#include <variant>

namespace Daybreak
{
	// TODO: Make this not static and add it to the project when available
	enum class AssetType
	{
		Texture2D,
		Animation,
		AnimationController
	};

	class AssetManager
	{
	public:
		template<typename T = U, typename std::enable_if<std::is_same<T, Animation>::value ||
															 std::is_same<T, AnimationController>::value ||
															 std::is_same<T, Texture2D>::value,
														 int>::type = 0>
		static Ref<T>& GetAssetRef(const std::filesystem::path& localFilepath)
		{
			return std::get<Ref<T>>(s_AssetMap[localFilepath]);
		}

		template<typename T>
		static void AddAssetRef(const Ref<T> asset, const std::filesystem::path& localFilepath);

		static bool HasAssetRef(const std::filesystem::path& localFilepath);
		static bool HasAssetRef(const Ref<Texture2D>& texture);
		static bool HasAssetRef(const Ref<Animation>& anim);
		static bool HasAssetRef(const Ref<AnimationController>& controller);

		static void Clear()
		{
			s_AssetMap.clear();
			s_AssetTypeMap.clear();
		}


		template<typename T>
		static const std::filesystem::path GetFilepathOfRef(const Ref<T> asset);

	private:
		static std::unordered_map<std::filesystem::path, std::variant<Ref<Texture2D>, Ref<Animation>, Ref<AnimationController>>> s_AssetMap;
		static std::unordered_map<std::filesystem::path, AssetType> s_AssetTypeMap;
	};
}