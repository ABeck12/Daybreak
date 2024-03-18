#pragma once

#include "Daybreak/Core/UUID.h"
#include "Daybreak/Renderer/Texture.h"
#include "Daybreak/Renderer/SubTexture.h"

#define YAML_CPP_DLL
#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>
#include <fstream>

#include <glm/glm.hpp>


namespace YAML
{
	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::mat4>
	{
		static Node encode(const glm::mat4& mat)
		{
			Node node;
			node.push_back(mat[0]);
			node.push_back(mat[1]);
			node.push_back(mat[2]);
			node.push_back(mat[3]);
			node.SetStyle(EmitterStyle::Flow);
			// node.push_back((uint64_t)uuid);
			return node;
		}

		static bool decode(const Node& node, glm::mat4& mat)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;
			mat[0] = node[0].as<glm::vec4>();
			mat[1] = node[1].as<glm::vec4>();
			mat[2] = node[2].as<glm::vec4>();
			mat[3] = node[3].as<glm::vec4>();

			return true;
		}
	};

	template<>
	struct convert<Daybreak::UUID>
	{
		static Node encode(const Daybreak::UUID& uuid)
		{
			Node node;
			node.push_back((uint64_t)uuid);
			return node;
		}

		static bool decode(const Node& node, Daybreak::UUID& uuid)
		{
			uuid = node.as<uint64_t>();
			return true;
		}
	};
}

namespace Daybreak
{
	inline YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const glm::mat4& m)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq;
		out << m[0] << m[1] << m[2] << m[3];
		out << YAML::EndSeq;
		return out;
	}

	// Temporary
	inline YAML::Emitter& operator<<(YAML::Emitter& out, const Ref<Texture2D>& texture)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Filepath" << YAML::Value << texture->GetFilepath();
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
		out << YAML::EndMap;
		out << YAML::EndMap;

		return out;
	}

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const Ref<SubTexture2D>& subtexture)
	{
		out << YAML::BeginMap;

		out << YAML::Key << "Texture2D" << YAML::Value << subtexture->GetTexture();

		out << YAML::Key << "Min" << subtexture->GetMin();
		out << YAML::Key << "Max" << subtexture->GetMax();

		out << YAML::EndMap;

		return out;
	}
}