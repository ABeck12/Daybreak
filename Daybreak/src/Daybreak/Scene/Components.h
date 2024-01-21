#pragma once

#include "dbpch.h"

#include "Daybreak/Renderer/Texture.h"
#include "Daybreak/Renderer/Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Daybreak
{
	struct TransformComponent
	{
		glm::vec3 Position = { 0.0f,0.0f,0.0f };
		glm::vec3 Rotation = { 0.0f,0.0f,0.0f };
		glm::vec3 Scale = { 1.0f,1.0f,1.0f };

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Position)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};
	
	struct TagComponent
	{
		std::string Tag;
	};

	struct SpriteRendererComponent
	{
		Ref<Texture2D> Sprite;
		glm::vec4 TintColor = { 1.0f,1.0f,1.0f,1.0f };
		float TilingFactor = 1.0f;
	};

	struct CameraComponent
	{
		Camera Camera;
		bool Primary = false; // Move this to the scene class
	};

	//struct ParentComponent
	//{
		// children entity ids here
		// maybe in some sort of map/hash table?
	//};

	//struct ChildComponent
	//{
		// parents entity id here
	//};
}