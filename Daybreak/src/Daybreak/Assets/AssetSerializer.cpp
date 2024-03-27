#include "dbpch.h"

#include "Daybreak/Assets/AssetSerializer.h"

#include "Daybreak/Utils/YamlConversion.h"
#include "Daybreak/Assets/AssetManager.h"
#include <filesystem>

#define DB_ASSET_DIR std::string("../Sandbox/assets/")

namespace Daybreak
{
	static std::string GetParentDirectory(const std::string& filepath, const std::string& directory)
	{
		std::filesystem::path path(filepath);

		auto assetsPos = path.string().find(directory);
		if (assetsPos != std::string::npos)
		{
			return path.string().substr(assetsPos + directory.length() + 1);
		}

		DB_CORE_ERROR("Path {} is not in the assets directory", filepath);
		return path.string();
	}

	static std::string ChangeFileExtension(const std::string& filePath, const std::string& newExtension)
	{
		size_t dotPos = filePath.find_last_of('.');
		if (dotPos != std::string::npos)
		{
			return filePath.substr(0, dotPos + 1) + newExtension;
		}
		else
		{
			return filePath + '.' + newExtension;
		}
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const Ref<Texture2D>& texture)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Filepath" << YAML::Value << GetParentDirectory(texture->GetFilepath(), "assets");
		out << YAML::Key << "Specifications";
		out << YAML::BeginMap;
		out << YAML::Key << "Width" << YAML::Value << texture->GetTexutreSpecifications().Width;
		out << YAML::Key << "Height" << YAML::Value << texture->GetTexutreSpecifications().Height;
		switch (texture->GetTexutreSpecifications().Format)
		{
			case ImageFormat::RGB:
				out << YAML::Key << "Format" << YAML::Value << "RGB";
				break;
			case ImageFormat::RGBA:
				out << YAML::Key << "Format" << YAML::Value << "RGBA";
				break;
			default:
				out << YAML::Key << "Format" << YAML::Value << "None";
				DB_CORE_ERROR("Can't serialize image type \"None\"");
				break;
		}
		switch (texture->GetTexutreSpecifications().Filter)
		{
			case TextureFilterType::Point:
				out << YAML::Key << "Filter" << YAML::Value << "Point";
				break;
			case TextureFilterType::Bilinear:
				out << YAML::Key << "Filter" << YAML::Value << "Bilinear";
				break;
		}
		out << YAML::Key << "PixelsPerUnit" << YAML::Value << texture->GetTexutreSpecifications().PixelsPerUnit;
		out << YAML::EndMap;
		out << YAML::EndMap;

		return out;
	}


	void AssetSerializer::SerializeSprite(const Ref<Texture2D>& texture, const std::string& localFilepath)
	{
		YAML::Emitter out;

		out << YAML::BeginMap;
		out << YAML::Key << "Texture2D" << YAML::Value << texture;
		out << YAML::EndMap;

		std::ofstream fout(DB_ASSET_DIR + localFilepath);
		fout << out.c_str();
	}

	void AssetSerializer::SerializeAnimation(const Ref<Animation>& anim, const std::string& localFilepath)
	{
		YAML::Emitter out;

		out << YAML::BeginMap;

		out << YAML::Key << "KeyFrames";
		out << YAML::BeginSeq;

		for (auto frame : anim->m_KeyFrames)
		{
			out << YAML::BeginMap;

			std::string outFilepath;
			if (AssetManager::HasAssetRef(frame.Sprite->GetTexture()))
			{
				outFilepath = AssetManager::GetFilepathOfRef(frame.Sprite->GetTexture());
			}
			else
			{
				std::filesystem::path spritePath = frame.Sprite->GetTexture()->GetFilepath();
				outFilepath = "sprites/" + ChangeFileExtension(spritePath.filename().string(), "sprite");
				AssetManager::AddAssetRef(frame.Sprite->GetTexture(), outFilepath);
			}
			SerializeSprite(frame.Sprite->GetTexture(), outFilepath);
			out << YAML::Key << "Sprite";
			out << YAML::BeginMap;
			out << YAML::Key << "Filepath" << YAML::Value << outFilepath;
			out << YAML::Key << "Min" << YAML::Key << frame.Sprite->GetMin();
			out << YAML::Key << "Max" << YAML::Key << frame.Sprite->GetMax();
			out << YAML::EndMap;

			out << YAML::Key << "Duration" << YAML::Value << frame.Duration;
			out << YAML::EndMap;
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(DB_ASSET_DIR + localFilepath);
		fout << out.c_str();
	}

	void AssetSerializer::SerializeAnimationController(const Ref<AnimationController>& controller, const std::string& localFilepath)
	{
		YAML::Emitter out;

		out << YAML::BeginMap;

		out << YAML::Key << "StartupAnimation" << YAML::Value << controller->m_StartupAnimation;
		out << YAML::Key << "Animations" << YAML::Value << YAML::BeginSeq;
		for (auto kv : controller->m_AnimationMap)
		{
			out << YAML::BeginMap;

			out << YAML::Key << "Name" << YAML::Value << kv.first;

			std::string filepath;
			if (AssetManager::HasAssetRef(kv.second))
			{
				filepath = AssetManager::GetFilepathOfRef(kv.second);
			}
			else
			{
				filepath = "animations/" + kv.first + ".anim";
				// TODO: Figure out some way to avoid naming conflicts
				AssetManager::AddAssetRef(kv.second, filepath);
			}
			out << YAML::Key << "Filepath" << YAML::Value << filepath;
			SerializeAnimation(kv.second, filepath);
			out << YAML::EndMap;
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(DB_ASSET_DIR + localFilepath);
		fout << out.c_str();
	}

	Ref<Texture2D> AssetSerializer::DeserializeSprite(const std::string& localFilepath)
	{
		if (AssetManager::HasAssetRef(localFilepath))
		{
			return AssetManager::GetAssetRef<Texture2D>(localFilepath);
		}

		YAML::Node data;
		try
		{
			data = YAML::LoadFile(DB_ASSET_DIR + localFilepath);
		}
		catch (const YAML::ParserException& e)
		{
			DB_CORE_ERROR("Failed to load .dbscn file '{0}'\n     {1}", (DB_ASSET_DIR + localFilepath), e.what());
		}

		auto texture2D = data["Texture2D"];

		std::string filepath = DB_ASSET_DIR + texture2D["Filepath"].as<std::string>();

		TextureSpecifications spec;
		spec.Height = texture2D["Specifications"]["Height"].as<uint32_t>();
		spec.Width = texture2D["Specifications"]["Width"].as<uint32_t>();

		if (texture2D["Specifications"]["Format"].as<std::string>() == "RGB")
			spec.Format = Daybreak::ImageFormat::RGB;
		else if (texture2D["Specifications"]["Format"].as<std::string>() == "RGBA")
			spec.Format = Daybreak::ImageFormat::RGBA;

		if (texture2D["Specifications"]["Filter"].as<std::string>() == "Point")
			spec.Filter = Daybreak::TextureFilterType::Point;
		else if (texture2D["Specifications"]["Filter"].as<std::string>() == "Bilinear")
			spec.Filter = Daybreak::TextureFilterType::Bilinear;
		spec.PixelsPerUnit = texture2D["Specifications"]["PixelsPerUnit"].as<uint32_t>();

		Ref<Texture2D> texture = Texture2D::Create(spec, filepath);
		AssetManager::AddAssetRef(texture, localFilepath);
		return texture;
	}

	Ref<Animation> AssetSerializer::DeserializeAnimation(const std::string& localFilepath)
	{
		if (AssetManager::HasAssetRef(localFilepath))
		{
			DB_LOG("Asset manager has file {}", localFilepath);
			return AssetManager::GetAssetRef<Animation>(localFilepath);
		}

		YAML::Node data;
		try
		{
			data = YAML::LoadFile(DB_ASSET_DIR + localFilepath);
		}
		catch (const YAML::ParserException& e)
		{
			DB_CORE_ERROR("Failed to load .dbscn file '{0}'\n     {1}", (DB_ASSET_DIR + localFilepath), e.what());
		}

		auto keyFrames = data["KeyFrames"];
		Ref<Animation> anim = CreateRef<Animation>();

		for (auto frame : keyFrames)
		{
			std::string spriteFile = frame["Sprite"]["Filepath"].as<std::string>();
			Ref<Texture2D> sprite;
			if (AssetManager::HasAssetRef(spriteFile))
			{
				sprite = AssetManager::GetAssetRef<Texture2D>(spriteFile);
			}
			else
			{
				sprite = DeserializeSprite(spriteFile);
			}
			glm::vec2 min = frame["Sprite"]["Min"].as<glm::vec2>();
			glm::vec2 max = frame["Sprite"]["Max"].as<glm::vec2>();
			float duration = frame["Duration"].as<float>();

			Ref<SubTexture2D> subsprite = CreateRef<SubTexture2D>(sprite, min, max);

			anim->AddKeyFrame({ subsprite, duration, nullptr });
		}
		AssetManager::AddAssetRef(anim, localFilepath);

		return anim;
	}

	Ref<AnimationController> AssetSerializer::DeserializeAnimationController(const std::string& localFilepath)
	{
		if (AssetManager::HasAssetRef(localFilepath))
		{
			DB_LOG("Asset manager has file {}", localFilepath);
			return AssetManager::GetAssetRef<AnimationController>(localFilepath);
		}

		YAML::Node data;
		try
		{
			data = YAML::LoadFile(DB_ASSET_DIR + localFilepath);
		}
		catch (const YAML::ParserException& e)
		{
			DB_CORE_ERROR("Failed to load .dbscn file '{0}'\n     {1}", (DB_ASSET_DIR + localFilepath), e.what());
		}

		Ref<AnimationController> controller = CreateRef<AnimationController>();
		std::string startupAnimation = data["StartupAnimation"].as<std::string>();

		auto animations = data["Animations"];

		for (auto anim : animations)
		{
			std::string name = anim["Name"].as<std::string>();
			std::string filepath = anim["Filepath"].as<std::string>();

			if (AssetManager::HasAssetRef(filepath))
			{
				controller->AddAnimation(name, AssetManager::GetAssetRef<Animation>(filepath));
			}
			else
			{
				controller->AddAnimation(name, DeserializeAnimation(filepath));
			}
		}
		controller->SetStartupAnimation(startupAnimation);

		AssetManager::AddAssetRef(controller, localFilepath);

		return controller;
	}
}