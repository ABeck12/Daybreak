#pragma once

#include "dbpch.h"

#include "Daybreak/Renderer/Texture.h"
#include "Daybreak/Renderer/Camera.h"
#include "Daybreak/Core/UUID.h"
#include "Daybreak/AssetSource/AnimationSource.h"

#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Daybreak
{
	struct IDComponent
	{
		UUID ID = 0;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
	};

	struct RelationshipComponent
	{
		UUID ParentID = 0;

		uint32_t AmountOfChildren = 0;
		std::vector<UUID> ChildrenIDs;

		RelationshipComponent() = default;
		RelationshipComponent(const RelationshipComponent&) = default;
	};

	struct TransformComponent
	{
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f }; // Rotation in radians
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		glm::mat4 GetTransform() const
		{
			return glm::translate(glm::mat4(1.0f), Position) * glm::toMat4(glm::quat(Rotation)) * glm::scale(glm::mat4(1.0f), Scale);
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
		uint32_t PixelsPerUnit = 128; //TODO: move to a sprite asset class
		// uint8_t RenderLayer = 0;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(Ref<Texture2D> sprite)
			: Sprite(sprite) {}
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

		uint16_t CollisionLayer = 0;
		bool IsTrigger = false;

		void* RuntimeFixture = nullptr;
		void* RuntimeBody = nullptr;

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
	};

	struct CircleCollider2DComponent
	{
		float Radius = 0.5f;
		glm::vec2 Offset = { 0.0f, 0.0f };

		uint16_t CollisionLayer = 0;
		bool IsTrigger = false;

		void* RuntimeFixture = nullptr;
		void* RuntimeBody = nullptr;

		CircleCollider2DComponent() = default;
		CircleCollider2DComponent(const CircleCollider2DComponent&) = default;
	};

	struct Rigidbody2DComponent
	{
		enum class BodyType
		{
			Static = 0,
			Dynamic,
			Kinematic
		};

		BodyType Type = BodyType::Static;
		bool ContinuousDetection = false;

		glm::vec2 Velocity = { 0.0f, 0.0f };

		bool FixedRotation = false;

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

	struct AnimatorComponent
	{
		bool IsPlaying = true;
		Ref<AnimationSource> Source;
		glm::vec4 TintColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		uint32_t PixelsPerUnit = 128; //TODO: move to a sprite asset class


		AnimatorComponent() = default;
		AnimatorComponent(const AnimatorComponent&) = default;
	};

	// Forward decleration
	class ScriptableEntity;

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []()
			{
				return static_cast<ScriptableEntity*>(new T());
			};
			DestroyScript = [](NativeScriptComponent* nsc)
			{
				delete nsc->Instance;
				nsc->Instance = nullptr;
			};
		}
	};


}