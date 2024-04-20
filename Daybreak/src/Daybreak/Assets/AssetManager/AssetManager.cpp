#include "dbpch.h"

#include "Daybreak/Assets/AssetManager/AssetManager.h"

#include "Daybreak/Utils/YamlConversion.h"
#include <yaml-cpp/yaml.h>

namespace Daybreak
{
	static YAML::Emitter& operator<<(YAML::Emitter& out, const Ref<Texture2D>& texture)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Filepath" << YAML::Value << std::filesystem::relative(texture->GetFilepath(), AssetManager::Get()->GetAssetDir());
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

	AssetManager* AssetManager::s_AssetManager = nullptr;

	void AssetManager::Init()
	{
		DB_CORE_ASSERT(!s_AssetManager, "AssetManager singleton already exists");
		s_AssetManager = new AssetManager();
		s_AssetManager->FindAssetsDirectory();
	}

	void AssetManager::Shutdown()
	{
		delete s_AssetManager;
		s_AssetManager = nullptr;
	}

	void AssetManager::FindAssetsDirectory()
	{
		std::filesystem::path currentSearchPath = std::filesystem::current_path();

		for (int i = 0; i < 50; i++) // Recursive search 10 times
		{
			for (auto searchPath : std::filesystem::recursive_directory_iterator(currentSearchPath))
			{
				if (!searchPath.is_directory())
					continue;
				std::filesystem::path path = searchPath;
				if (path.stem() == "assets" && path.parent_path().stem() == "Sandbox")
				{
					m_AssetDirectoryPath = path;
					DB_CORE_INFO("Found Assets dirctory at {}", m_AssetDirectoryPath);
					return;
				}
				currentSearchPath = currentSearchPath.parent_path();
			}
		}
		if (std::filesystem::exists("../../../Sandbox/assets"))
		{
			m_AssetDirectoryPath = std::filesystem::absolute("../../../Sandbox/assets");
			DB_CORE_INFO("Found Assets dirctory at {}", m_AssetDirectoryPath);
			return;
		}
		DB_CORE_ASSERT(false, "Could not find asset directory");
	}

	bool AssetManager::HasAssetPath(const std::filesystem::path& path)
	{
		if (m_Assets.Texture2Ds.find(path) != m_Assets.Texture2Ds.end())
			return true;
		else if (m_Assets.Animations.find(path) != m_Assets.Animations.end())
			return true;
		else if (m_Assets.AnimationControllers.find(path) != m_Assets.AnimationControllers.end())
			return true;
		else if (m_Assets.Shaders.find(path) != m_Assets.Shaders.end())
			return true;
		return false;
	}

	std::filesystem::path AssetManager::GetTexture2DFilepath(const Ref<Texture2D>& texture)
	{
		for (const auto kv : m_Assets.Texture2Ds)
		{
			if (kv.second == texture)
			{
				return kv.first;
			}
		}
		return std::filesystem::relative(texture->GetFilepath(), m_AssetDirectoryPath).replace_extension("sprite");
	}

	std::filesystem::path AssetManager::GetAnimationFilepath(const Ref<Animation>& anim)
	{
		for (const auto kv : m_Assets.Animations)
		{
			if (kv.second == anim)
			{
				return kv.first;
			}
		}
		return std::filesystem::path("animations/" + anim->m_Name + ".anim");
	}

	std::filesystem::path AssetManager::GetAnimationControllerFilepath(const Ref<AnimationController>& controller)
	{
		for (const auto kv : m_Assets.AnimationControllers)
		{
			if (kv.second == controller)
			{
				return kv.first;
			}
		}
		return "animations/unknown.controller";
	}

	Ref<Shader> AssetManager::LoadShader(const std::filesystem::path& assetPath)
	{
		auto loc = m_Assets.Shaders.find(m_AssetDirectoryPath / assetPath);
		if (loc != m_Assets.Shaders.end())
		{
			return m_Assets.Shaders[assetPath];
		}
		Ref<Shader> newShader = Shader::Create(assetPath.stem().string(), m_AssetDirectoryPath / assetPath);
		m_Assets.Shaders[assetPath] = newShader;
		return newShader;
	}

	Ref<Texture2D> AssetManager::LoadTexture2D(const std::filesystem::path& assetPath)
	{
		auto loc = m_Assets.Texture2Ds.find(assetPath);
		if (loc != m_Assets.Texture2Ds.end())
		{
			return m_Assets.Texture2Ds[assetPath];
		}
		Ref<Texture2D> texture = DeserializeTexture2D(m_AssetDirectoryPath / assetPath);
		m_Assets.Texture2Ds[assetPath] = texture;
		return texture;
	}

	Ref<Animation> AssetManager::LoadAnimation(const std::filesystem::path& assetPath)
	{
		auto loc = m_Assets.Animations.find(assetPath);
		if (loc != m_Assets.Animations.end())
		{
			return m_Assets.Animations[assetPath];
		}
		Ref<Animation> anim = DeserializeAnimation(m_AssetDirectoryPath / assetPath);
		m_Assets.Animations[assetPath] = anim;
		return anim;
	}

	Ref<AnimationController> AssetManager::LoadAnimationController(const std::filesystem::path& assetPath)
	{
		auto loc = m_Assets.AnimationControllers.find(assetPath);
		if (loc != m_Assets.AnimationControllers.end())
		{
			return m_Assets.AnimationControllers[assetPath];
		}
		Ref<AnimationController> controller = DeserializeAnimationController(m_AssetDirectoryPath / assetPath);
		m_Assets.AnimationControllers[assetPath] = controller;
		return controller;
	}

	Ref<Texture2D> AssetManager::DeserializeTexture2D(const std::filesystem::path& filepath)
	{
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filepath.string());
		}
		catch (const YAML::ParserException& e)
		{
			DB_CORE_ERROR("Failed to load .sprite file '{0}'\n     {1}", filepath, e.what());
		}

		auto texture2D = data["Texture2D"];

		std::filesystem::path texturePath = AssetManager::Get()->GetAssetDir() / texture2D["Filepath"].as<std::filesystem::path>();

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

		Ref<Texture2D> texture = Texture2D::Create(spec, texturePath);
		return texture;
	}

	Ref<Animation> AssetManager::DeserializeAnimation(const std::filesystem::path& filepath)
	{
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filepath.string());
		}
		catch (const YAML::ParserException& e)
		{
			DB_CORE_ERROR("Failed to load .anim file '{0}'\n     {1}", filepath, e.what());
		}

		Ref<Animation> anim = CreateRef<Animation>(data["Name"].as<std::string>());
		auto keyFrames = data["KeyFrames"];

		for (auto frame : keyFrames)
		{
			std::string spriteFile = frame["Sprite"]["Filepath"].as<std::string>();
			DB_LOG("spriteFile {}", spriteFile);
			Ref<Texture2D> sprite = LoadTexture2D(spriteFile);
			glm::vec2 min = frame["Sprite"]["Min"].as<glm::vec2>();
			glm::vec2 max = frame["Sprite"]["Max"].as<glm::vec2>();
			float duration = frame["Duration"].as<float>();

			Ref<SubTexture2D> subsprite = CreateRef<SubTexture2D>(sprite, min, max);

			anim->AddKeyFrame({ subsprite, duration, nullptr });
		}

		return anim;
	}

	Ref<AnimationController> AssetManager::DeserializeAnimationController(const std::filesystem::path& filepath)
	{
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filepath.string());
		}
		catch (const YAML::ParserException& e)
		{
			DB_CORE_ERROR("Failed to load .controller file '{0}'\n     {1}", filepath, e.what());
		}

		Ref<AnimationController> controller = CreateRef<AnimationController>();
		std::string startupAnimation = data["StartupAnimation"].as<std::string>();

		auto animations = data["Animations"];

		for (auto anim : animations)
		{
			std::string name = anim["Name"].as<std::string>();
			std::string animPath = anim["Filepath"].as<std::string>();

			Ref<Animation> animation = LoadAnimation(animPath);
			controller->AddAnimation(animation);
		}
		controller->SetStartupAnimation(startupAnimation);

		return controller;
	}

	void AssetManager::SerializeTexture2D(const Ref<Texture2D>& texture)
	{
		std::filesystem::path filepath = texture->GetFilepath();
		SerializeTexture2D(texture, std::filesystem::relative(filepath.replace_extension("sprite"), m_AssetDirectoryPath));
	}

	void AssetManager::SerializeTexture2D(const Ref<Texture2D>& texture, const std::filesystem::path& assetPath)
	{
		YAML::Emitter out;

		out << YAML::BeginMap;
		out << YAML::Key << "Texture2D" << YAML::Value << texture;
		out << YAML::EndMap;

		std::ofstream fout(m_AssetDirectoryPath / assetPath);
		fout << out.c_str();
	}

	void AssetManager::SerializeAnimation(const Ref<Animation>& anim, const std::filesystem::path& assetPath)
	{
		YAML::Emitter out;

		out << YAML::BeginMap;

		out << YAML::Key << "Name" << YAML::Value << anim->GetName();

		out << YAML::Key << "KeyFrames";
		out << YAML::BeginSeq;

		for (const KeyFrame frame : anim->GetKeyFrames())
		{
			out << YAML::BeginMap;

			std::filesystem::path texturePath = GetTexture2DFilepath(frame.Sprite->GetTexture());
			SerializeTexture2D(frame.Sprite->GetTexture(), texturePath);
			out << YAML::Key << "Sprite";
			out << YAML::BeginMap;
			out << YAML::Key << "Filepath" << YAML::Value << texturePath;
			out << YAML::Key << "Min" << YAML::Key << frame.Sprite->GetMin();
			out << YAML::Key << "Max" << YAML::Key << frame.Sprite->GetMax();
			out << YAML::EndMap;

			out << YAML::Key << "Duration" << YAML::Value << frame.Duration;
			out << YAML::EndMap;
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(m_AssetDirectoryPath / assetPath);
		fout << out.c_str();
	}

	void AssetManager::SerializeAnimationController(const Ref<AnimationController>& controller, const std::filesystem::path& assetPath)
	{
		YAML::Emitter out;

		out << YAML::BeginMap;

		out << YAML::Key << "StartupAnimation" << YAML::Value << controller->m_StartupAnimation;
		out << YAML::Key << "Animations" << YAML::Value << YAML::BeginSeq;
		for (auto kv : controller->m_AnimationMap)
		{
			out << YAML::BeginMap;

			out << YAML::Key << "Name" << YAML::Value << kv.first;

			std::filesystem::path animationAssetPath = GetAnimationFilepath(kv.second);
			if (animationAssetPath.filename() == "unknown")
			{
				animationAssetPath.replace_filename(kv.first);
			}
			out << YAML::Key << "Filepath" << YAML::Value << animationAssetPath;
			SerializeAnimation(kv.second, animationAssetPath);
			out << YAML::EndMap;
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;
		DB_LOG(m_AssetDirectoryPath / assetPath);
		std::ofstream fout(m_AssetDirectoryPath / assetPath);
		fout << out.c_str();
	}
}