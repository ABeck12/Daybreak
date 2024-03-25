#include "dbpch.h"

#include "Daybreak/Renderer/Renderer2D.h"
#include "Daybreak/Renderer/RenderCommand.h"
#include "Daybreak/Renderer/Shader.h"
#include "Daybreak/Renderer/Texture.h"

namespace Daybreak
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec2 TexCoord;
		glm::vec4 Color;
		float TexIndex;
		float TilingFactor;

		// Editor-only
		int EntityID;
	};

	struct LineVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;

		// Editor-Only;
		// int EntityID;
	};

	struct PointLightVertex
	{
		glm::vec3 WorldPosition;
		glm::vec3 LocalPosition;
		glm::vec3 Color;
		float Radius;
		float Intensity;
		// float Falloff;
	};

	struct Renderer2DData
	{
		static const uint32_t MaxTextureSlots = 32;
		static const uint32_t MaxQuads = 10000;
		static const uint32_t MaxVertices = 4 * MaxQuads;
		static const uint32_t MaxIndices = 6 * MaxQuads;

		glm::mat4 ViewProjectionMatrix;

		// For Quads
		Ref<VertexBuffer> QuadVB;
		Ref<VertexArray> QuadVA;
		Ref<Shader> QuadShader;
		Ref<Texture2D> WhiteTexture;

		// For Lines
		Ref<VertexBuffer> LineVB;
		Ref<VertexArray> LineVA;
		Ref<Shader> LineShader;
		float DefaultLineWidth = 1.0f;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		uint32_t LineVertexCount = 0;
		LineVertex* LineVertexBufferBase = nullptr;
		LineVertex* LineVertexBufferPtr = nullptr;

		glm::vec4 QuadVertexPositions[4];

		uint32_t TextureSlotIndex = 1;
		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;

		// ========
		// Lighting
		// ========

		// Point Lights
		Ref<VertexBuffer> PointLightVB;
		Ref<VertexArray> PointLightVA;
		Ref<Shader> PointLightShader;

		uint32_t PointLightVertexCount = 0;
		PointLightVertex* PointLightVertexBufferBase = nullptr;
		PointLightVertex* PointLightVertexBufferPtr = nullptr;
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		// Quads
		s_Data.QuadShader = Shader::Create("Renderer2D_QuadShader", "../Sandbox/assets/shaders/Renderer2D_QuadShader.glsl");

		s_Data.QuadVA = VertexArray::Create();
		s_Data.QuadVB = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		s_Data.QuadVB->SetLayout({
			{ RenderDataTypes::Float3, std::string("a_Position") },
			{ RenderDataTypes::Float2, std::string("a_TexCoord") },
			{ RenderDataTypes::Float4, std::string("a_Color") },
			{ RenderDataTypes::Float, std::string("a_TexIndex") },
			{ RenderDataTypes::Float, std::string("a_TilingFactor") },
			{ RenderDataTypes::Int, std::string("a_EntityID") },
		});
		s_Data.QuadVA->AddVertexBuffer(s_Data.QuadVB);
		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 3;

			quadIndices[i + 3] = offset + 1;
			quadIndices[i + 4] = offset + 2;
			quadIndices[i + 5] = offset + 3;

			offset += 4;
		}
		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVA->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		s_Data.WhiteTexture = Texture2D::Create({ 1, 1, Daybreak::ImageFormat::RGBA, Daybreak::TextureFilterType::Point });
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };

		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		s_Data.QuadShader->Bind();
		s_Data.QuadShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);


		// Lines
		s_Data.LineShader = Shader::Create("Renderer2D_LineShader", "../Sandbox/assets/shaders/Renderer2D_LineShader.glsl");

		s_Data.LineVA = VertexArray::Create();
		s_Data.LineVB = VertexBuffer::Create(s_Data.MaxVertices * sizeof(LineVertex));
		s_Data.LineVB->SetLayout({ { RenderDataTypes::Float3, std::string("a_Position") },
								   { RenderDataTypes::Float4, std::string("a_Color") } });
		s_Data.LineVA->AddVertexBuffer(s_Data.LineVB);
		s_Data.LineVertexBufferBase = new LineVertex[s_Data.MaxVertices];


		// Lighting
		// s_Data.PointLightShader = Shader::Create("Renderer2D_PointLightShader", "../Sandbox/assets/shaders/Renderer2D_PointLightShader.glsl");
		s_Data.PointLightShader = Shader::Create("Renderer2D_PointLightShader", "../Sandbox/assets/shaders/PointLightShader2.glsl");
		s_Data.PointLightVA = VertexArray::Create();
		s_Data.PointLightVB = VertexBuffer::Create(s_Data.MaxVertices * sizeof(PointLightVertex));
		s_Data.PointLightVB->SetLayout({
			{ RenderDataTypes::Float3, std::string("a_WorldPosition") },
			{ RenderDataTypes::Float3, std::string("a_LocalPosition") },
			{ RenderDataTypes::Float3, std::string("a_Color") },
			{ RenderDataTypes::Float, std::string("a_Radius") },
			{ RenderDataTypes::Float, std::string("a_Intensity") },
			// { RenderDataTypes::Float, std::string("a_Falloff") },
		});
		s_Data.PointLightVA->AddVertexBuffer(s_Data.PointLightVB);
		s_Data.PointLightVA->SetIndexBuffer(quadIB);
		s_Data.PointLightVertexBufferBase = new PointLightVertex[s_Data.MaxVertices];
	}

	void Renderer2D::Shutdown()
	{
		delete[] s_Data.QuadVertexBufferBase;
		delete[] s_Data.LineVertexBufferBase;

		delete[] s_Data.PointLightVertexBufferBase;
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		s_Data.ViewProjectionMatrix = camera.GetProjection() * transform;

		StartBatch();
	}

	void Renderer2D::EndScene()
	{
		Flush();
	}

	void Renderer2D::Flush()
	{
		if (s_Data.QuadIndexCount > 0)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
			s_Data.QuadVB->SetData(s_Data.QuadVertexBufferBase, dataSize);

			s_Data.QuadShader->Bind();
			s_Data.QuadShader->SetMat4("u_ViewProjection", s_Data.ViewProjectionMatrix);
			for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
				s_Data.TextureSlots[i]->Bind(i);
			RenderCommand::DrawIndexed(s_Data.QuadVA, s_Data.QuadIndexCount);
		}
		if (s_Data.LineVertexCount > 0)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.LineVertexBufferPtr - (uint8_t*)s_Data.LineVertexBufferBase);
			s_Data.LineVB->SetData(s_Data.LineVertexBufferBase, dataSize);

			s_Data.LineShader->Bind();
			s_Data.LineShader->SetMat4("u_ViewProjection", s_Data.ViewProjectionMatrix);
			RenderCommand::SetLineWidth(s_Data.DefaultLineWidth);
			RenderCommand::DrawLines(s_Data.LineVA, s_Data.LineVertexCount);
		}
		if (s_Data.PointLightVertexCount > 0)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.PointLightVertexBufferPtr - (uint8_t*)s_Data.PointLightVertexBufferBase);
			s_Data.PointLightVB->SetData(s_Data.PointLightVertexBufferBase, dataSize);

			s_Data.PointLightShader->Bind();
			s_Data.PointLightShader->SetMat4("u_ViewProjection", s_Data.ViewProjectionMatrix);

			RenderCommand::DrawIndexed(s_Data.PointLightVA, s_Data.PointLightVertexCount);
		}
	}

	void Renderer2D::StartBatch()
	{
		s_Data.TextureSlotIndex = 1;

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.LineVertexCount = 0;
		s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;

		s_Data.PointLightVertexCount = 0;
		s_Data.PointLightVertexBufferPtr = s_Data.PointLightVertexBufferBase;
	}

	void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tintColor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawQuad(transform, texture, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawQuad(transform, s_Data.WhiteTexture, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, const SpriteRendererComponent& spriteRenderer, int entityID)
	{
		if (spriteRenderer.Sprite)
			DrawQuad(glm::scale(transform, { (float)spriteRenderer.Sprite->GetWidth() / (float)spriteRenderer.Sprite->GetTexutreSpecifications().PixelsPerUnit, (float)spriteRenderer.Sprite->GetHeight() / (float)spriteRenderer.Sprite->GetTexutreSpecifications().PixelsPerUnit, 1.0f }),
					 spriteRenderer.Sprite, spriteRenderer.TintColor, spriteRenderer.TilingFactor, (int)entityID);
		else
			DrawQuad(transform, s_Data.WhiteTexture, spriteRenderer.TintColor, spriteRenderer.TilingFactor, entityID);
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, const AnimatorComponent& anim, int entityID)
	{
		auto currentFrame = anim.Controller->GetActiveAnimation()->GetCurrentKeyFrame();
		TextureSpecifications spec = currentFrame.Sprite->GetTexture()->GetTexutreSpecifications();
		Renderer2D::DrawQuad(glm::scale(transform, { (float)currentFrame.Sprite->GetWidth() / (float)spec.PixelsPerUnit, (float)currentFrame.Sprite->GetHeight() / (float)spec.PixelsPerUnit, 1.0f }),
							 currentFrame.Sprite, anim.TintColor, 1.0f, (int)entityID);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& tintColor, const float& tilingFactor, int entityID)
	{
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i] == *texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f && texture != s_Data.WhiteTexture)
		{
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				NextBatch();
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}
		// float tilingFactor = 0.0f;
		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}
		s_Data.QuadIndexCount += 6;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, subtexture, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, const glm::vec4& tintColor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawQuad(transform, subtexture, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subtexture, const glm::vec4& tintColor, const float& tilingFactor, int entityID)
	{
		constexpr size_t quadVertexCount = 4;
		const Ref<Texture2D> texture = subtexture->GetTexture();

		glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		std::copy(subtexture->GetTextureCoords(), subtexture->GetTextureCoords() + quadVertexCount, textureCoords);

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i] == *texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f && texture != s_Data.WhiteTexture)
		{
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				NextBatch();
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}
		// float tilingFactor = 0.0f;
		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}
		s_Data.QuadIndexCount += 6;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float& rotDeg, const Ref<Texture2D>& texture, const glm::vec4& tintColor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(rotDeg), { 0.0f, 0.0f, 1.0f });
		DrawQuad(transform * rotate, texture, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float& rotDeg, const Ref<Texture2D>& texture, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotDeg, texture, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float& rotDeg, const Ref<SubTexture2D>& subtexture, const glm::vec4& tintColor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(rotDeg), { 0.0f, 0.0f, 1.0f });
		DrawQuad(transform * rotate, subtexture, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float& rotDeg, const Ref<SubTexture2D>& subtexture, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotDeg, subtexture, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float& rotDeg, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(rotDeg), { 0.0f, 0.0f, 1.0f });
		DrawQuad(rotate * transform, s_Data.WhiteTexture, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float& rotDeg, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotDeg, color);
	}

	void Renderer2D::DrawLine(const glm::vec3& pos1, const glm::vec3& pos2, const glm::vec4& color)
	{
		s_Data.LineVertexBufferPtr->Position = pos1;
		s_Data.LineVertexBufferPtr->Color = color;
		// s_Data.QuadVertexBufferPtr->EntityID = entityID;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineVertexBufferPtr->Position = pos2;
		s_Data.LineVertexBufferPtr->Color = color;
		// s_Data.QuadVertexBufferPtr->EntityID = entityID;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineVertexCount += 2;
	}

	void Renderer2D::DrawLine(const glm::vec2& pos1, const glm::vec2& pos2, const glm::vec4& color)
	{
		DrawLine({ pos1.x, pos1.y, 0.0f }, { pos2.x, pos2.y, 0.0f }, color);
	}

	void Renderer2D::DrawPointLight(const glm::vec3& position, float radius, float intensity, const glm::vec3& color)
	{
		glm::mat4 translation = glm::translate(glm::mat4(1.0), position);
		glm::mat4 scale = glm::scale(glm::mat4(1.0), glm::vec3(2 * radius));
		for (size_t i = 0; i < 4; i++)
		{
			s_Data.PointLightVertexBufferPtr->WorldPosition = (scale * translation) * s_Data.QuadVertexPositions[i];
			s_Data.PointLightVertexBufferPtr->LocalPosition = s_Data.QuadVertexPositions[i] * 2.0f;
			s_Data.PointLightVertexBufferPtr->Color = color;
			// s_Data.PointLightVertexBufferPtr->Color = { 1.0, 0.5, 0.2 };
			s_Data.PointLightVertexBufferPtr->Radius = radius;
			s_Data.PointLightVertexBufferPtr->Intensity = intensity;
			s_Data.PointLightVertexBufferPtr++;
		}
		s_Data.PointLightVertexCount += 6;
	}
}