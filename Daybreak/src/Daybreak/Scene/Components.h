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
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f }; // Rotation in radians
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		glm::mat4 GetTransform() const
		{
			return glm::translate(glm::mat4(1.0f), Position)
				* glm::toMat4(glm::quat(Rotation))
				* glm::scale(glm::mat4(1.0f), Scale);
		}

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
	};
	
	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
	};

	struct SpriteRendererComponent
	{
		Ref<Texture2D> Sprite;
		glm::vec4 TintColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		float TilingFactor = 1.0f;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
	};

	struct CameraComponent
	{
		Camera Camera;
		bool Primary = false; // Move this to the scene class

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct BoxCollider2DComponent
	{
		glm::vec2 Size = { 0.5f, 0.5f };
		glm::vec2 Offset = { 0.0f, 0.0f };
		//bool IsTrigger = false;

		//void* RuntimeFixture = nullptr;

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
	};

	struct Rigidbody2DComponent
	{
		enum class BodyType { Static = 0, Dynamic, Kinematic };

		BodyType Type = BodyType::Static;
		bool ContinuousDetection = false;
		
		glm::vec2 Velocity = { 0.0f, 0.0f };

		bool FixedRotation = false;
		//bool FixedPositionX = false;
		//bool FixedPositionY = false;
		
		float Mass = 1.0f;
		float GravityScale = 1.0f;
		float AngularDrag = 1.0f;
		float LinearDrag = 1.0f;

		float Density = 1.0f;
		float Friction = 1.0f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		void* RuntimeBody = nullptr;

		Rigidbody2DComponent() = default;
		Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
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