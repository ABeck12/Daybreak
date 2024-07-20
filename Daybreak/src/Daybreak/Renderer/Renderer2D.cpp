#include "dbpch.h"

#include "Daybreak/Renderer/Renderer2D.h"
#include "Daybreak/Renderer/RenderCommand.h"
#include "Daybreak/Renderer/Shader.h"
#include "Daybreak/Renderer/Texture.h"
#include "Daybreak/Renderer/MSDFData.h"
#include "Daybreak/Renderer/UniformBuffer.h"

#include "Daybreak/Assets/AssetManager/AssetManager.h"

namespace Daybreak
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec2 TexCoord;
		glm::vec4 Color;
		float TexIndex;
		float TilingFactor;
		int EntityID;
	};

	struct CircleVertex
	{
		glm::vec3 WorldPosition;
		glm::vec3 LocalPosition;
		glm::vec4 Color;
		float Fade;
		float Thickness;
		int EntityID;
	};

	struct LineVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;

		// Editor-Only;
		// int EntityID;
	};

	struct TextVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;

		// TODO: bg color for outline/bg

		// Editor-only
		int EntityID;
	};


	struct Renderer2DData
	{
		static const uint32_t MaxTextureSlots = 32;
		static const uint32_t MaxQuads = 1000;
		static const uint32_t MaxVertices = 4 * MaxQuads;
		static const uint32_t MaxIndices = 6 * MaxQuads;
		static const uint32_t MaxLines = 250 * 2;

		// For Quads
		Ref<VertexBuffer> QuadVB;
		Ref<VertexArray> QuadVA;
		Ref<Shader> QuadShader;
		Ref<Texture2D> WhiteTexture;

		// For Circles
		Ref<VertexBuffer> CircleVB;
		Ref<VertexArray> CircleVA;
		Ref<Shader> CircleShader;

		// For Lines
		Ref<VertexBuffer> LineVB;
		Ref<VertexArray> LineVA;
		Ref<Shader> LineShader;
		const float DefaultLineWidth = 1.0f;

		Ref<VertexArray> TextVertexArray;
		Ref<VertexBuffer> TextVertexBuffer;
		Ref<Shader> TextShader;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		uint32_t CircleIndexCount = 0;
		CircleVertex* CircleVertexBufferBase = nullptr;
		CircleVertex* CircleVertexBufferPtr = nullptr;

		uint32_t LineVertexCount = 0;
		LineVertex* LineVertexBufferBase = nullptr;
		LineVertex* LineVertexBufferPtr = nullptr;

		uint32_t TextIndexCount = 0;
		TextVertex* TextVertexBufferBase = nullptr;
		TextVertex* TextVertexBufferPtr = nullptr;
		Ref<Texture2D> FontAtlasTexture;

		glm::vec4 QuadVertexPositions[4];

		uint32_t TextureSlotIndex = 1;
		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;

		struct CameraData
		{
			glm::mat4 ViewProjection;
		};
		CameraData CameraBuffer;
		Ref<UniformBuffer> CameraUniformBuffer;
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		// Quads
		s_Data.QuadShader = AssetManager::Get()->LoadShader("shaders/Renderer2D_QuadShader.glsl");

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
		s_Data.QuadShader->Unbind();

		// Cirlces
		s_Data.CircleShader = AssetManager::Get()->LoadShader("shaders/Renderer2D_CircleShader.glsl");
		s_Data.CircleVA = VertexArray::Create();
		s_Data.CircleVB = VertexBuffer::Create(s_Data.MaxVertices * sizeof(CircleVertex));
		s_Data.CircleVB->SetLayout({
			{ RenderDataTypes::Float3, std::string("a_WorldPosition") },
			{ RenderDataTypes::Float3, std::string("a_LocalPosition") },
			{ RenderDataTypes::Float4, std::string("a_Color") },
			{ RenderDataTypes::Float, std::string("a_Fade") },
			{ RenderDataTypes::Float, std::string("a_Thickness") },
			{ RenderDataTypes::Int, std::string("a_EntityID") },
		});
		s_Data.CircleVA->AddVertexBuffer(s_Data.CircleVB);
		s_Data.CircleVertexBufferBase = new CircleVertex[s_Data.MaxVertices];

		s_Data.CircleVA->SetIndexBuffer(quadIB);


		// Lines
		s_Data.LineShader = AssetManager::Get()->LoadShader("shaders/Renderer2D_LineShader.glsl");

		s_Data.LineVA = VertexArray::Create();
		s_Data.LineVB = VertexBuffer::Create(s_Data.MaxLines * sizeof(LineVertex));
		s_Data.LineVB->SetLayout({ { RenderDataTypes::Float3, std::string("a_Position") },
								   { RenderDataTypes::Float4, std::string("a_Color") } });
		s_Data.LineVA->AddVertexBuffer(s_Data.LineVB);
		s_Data.LineVertexBufferBase = new LineVertex[s_Data.MaxVertices];

		// Text
		s_Data.TextVertexArray = VertexArray::Create();

		s_Data.TextVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(TextVertex));
		s_Data.TextVertexBuffer->SetLayout({
			{ RenderDataTypes::Float3, "a_Position" },
			{ RenderDataTypes::Float4, "a_Color" },
			{ RenderDataTypes::Float2, "a_TexCoord" },
			{ RenderDataTypes::Int, "a_EntityID" },
		});
		s_Data.TextVertexArray->AddVertexBuffer(s_Data.TextVertexBuffer);
		s_Data.TextVertexArray->SetIndexBuffer(quadIB);
		s_Data.TextVertexBufferBase = new TextVertex[s_Data.MaxVertices];

		s_Data.TextShader = AssetManager::Get()->LoadShader("shaders/Renderer2D_TextShader.glsl");

		s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DData::CameraData), 0);
	}

	void Renderer2D::Shutdown()
	{
		delete[] s_Data.QuadVertexBufferBase;
		delete[] s_Data.CircleVertexBufferBase;
		delete[] s_Data.LineVertexBufferBase;
		delete[] s_Data.TextVertexBufferBase;
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		s_Data.CameraBuffer.ViewProjection = camera.GetProjection() * transform;
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));
		StartBatch();
	}

	void Renderer2D::EndScene()
	{
		Flush();
	}

	void Renderer2D::Flush()
	{
		if (s_Data.QuadIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
			s_Data.QuadVB->SetData(s_Data.QuadVertexBufferBase, dataSize);

			s_Data.QuadShader->Bind();
			for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
				s_Data.TextureSlots[i]->Bind(i);
			RenderCommand::DrawIndexed(s_Data.QuadVA, s_Data.QuadIndexCount);
		}
		if (s_Data.CircleIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CircleVertexBufferPtr - (uint8_t*)s_Data.CircleVertexBufferBase);
			s_Data.CircleVB->SetData(s_Data.CircleVertexBufferBase, dataSize);

			s_Data.CircleShader->Bind();
			RenderCommand::DrawIndexed(s_Data.CircleVA, s_Data.CircleIndexCount);
		}
		if (s_Data.LineVertexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.LineVertexBufferPtr - (uint8_t*)s_Data.LineVertexBufferBase);
			s_Data.LineVB->SetData(s_Data.LineVertexBufferBase, dataSize);

			s_Data.LineShader->Bind();
			RenderCommand::SetLineWidth(s_Data.DefaultLineWidth);
			RenderCommand::DrawLines(s_Data.LineVA, s_Data.LineVertexCount);
		}
		if (s_Data.TextIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.TextVertexBufferPtr - (uint8_t*)s_Data.TextVertexBufferBase);
			s_Data.TextVertexBuffer->SetData(s_Data.TextVertexBufferBase, dataSize);

			auto buf = s_Data.TextVertexBufferBase;
			s_Data.FontAtlasTexture->Bind(0);

			s_Data.TextShader->Bind();
			RenderCommand::DrawIndexed(s_Data.TextVertexArray, s_Data.TextIndexCount);
		}
	}

	void Renderer2D::StartBatch()
	{
		s_Data.TextureSlotIndex = 1;

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.CircleIndexCount = 0;
		s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;

		s_Data.LineVertexCount = 0;
		s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;

		s_Data.TextIndexCount = 0;
		s_Data.TextVertexBufferPtr = s_Data.TextVertexBufferBase;
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

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& tintColor, const float tilingFactor, int entityID)
	{
		DrawQuad(transform, s_Data.WhiteTexture, tintColor, tilingFactor, entityID);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& tintColor, const float tilingFactor, int entityID)
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

	void Renderer2D::DrawQuad(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& p4,
							  const glm::vec4& tintColor)
	{
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		glm::vec3 coords[4] = { p1, p2, p3, p4 };

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = coords[i];
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Data.QuadVertexBufferPtr->TexIndex = 0.0f;
			s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
			s_Data.QuadVertexBufferPtr->EntityID = -1;
			s_Data.QuadVertexBufferPtr++;
		}
		s_Data.QuadIndexCount += 6;
	}

	void Renderer2D::DrawQuad(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const glm::vec2& p4,
							  const glm::vec4& tintColor)
	{
		Renderer2D::DrawQuad(glm::vec3(p1, 0), glm::vec3(p2, 0), glm::vec3(p3, 0), glm::vec3(p4, 0), tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, subtexture, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, const glm::vec4& tintColor)
	{
		const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawQuad(transform, subtexture, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subtexture, const glm::vec4& tintColor, const float tilingFactor, int entityID)
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

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float rotDeg, const Ref<Texture2D>& texture, const glm::vec4& tintColor)
	{
		const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		const glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(rotDeg), { 0.0f, 0.0f, 1.0f });
		DrawQuad(transform * rotate, texture, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float rotDeg, const Ref<Texture2D>& texture, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotDeg, texture, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float rotDeg, const Ref<SubTexture2D>& subtexture, const glm::vec4& tintColor)
	{
		const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		const glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(rotDeg), { 0.0f, 0.0f, 1.0f });
		DrawQuad(transform * rotate, subtexture, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float rotDeg, const Ref<SubTexture2D>& subtexture, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotDeg, subtexture, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float rotDeg, const glm::vec4& color)
	{
		const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		const glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(rotDeg), { 0.0f, 0.0f, 1.0f });
		DrawQuad(rotate * transform, s_Data.WhiteTexture, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float rotDeg, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotDeg, color);
	}

	void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, const float fade, const float thickness)
	{
		constexpr size_t quadVertexCount = 4;

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			// DB_LOG("Vertex position {} Center position {}", transform * s_Data.QuadVertexPositions[i], transform * glm::vec4(1) - glm::vec4(1));
			s_Data.CircleVertexBufferPtr->WorldPosition = transform * s_Data.QuadVertexPositions[i];
			s_Data.CircleVertexBufferPtr->LocalPosition = s_Data.QuadVertexPositions[i] * 2.0f;
			s_Data.CircleVertexBufferPtr->Color = color;
			s_Data.CircleVertexBufferPtr->Fade = fade;
			s_Data.CircleVertexBufferPtr->Thickness = thickness;

			s_Data.CircleVertexBufferPtr++;
		}
		s_Data.CircleIndexCount += 6;
	}

	void Renderer2D::DrawCircle(const glm::vec3& position, const float radius, const glm::vec4& color, const float fade, const float thickness)
	{
		const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { radius * 2, radius * 2, 1.0f });
		DrawCircle(transform, color, fade, thickness);
	}

	void Renderer2D::DrawCircle(const glm::vec2& position, float radius, const glm::vec4& color, const float fade, const float thickness)
	{
		DrawCircle(glm::vec3(position.x, position.y, 0), radius, color, fade, thickness);
	}

	void Renderer2D::DrawLine(const glm::vec3& pos1, const glm::vec3& pos2, const glm::vec4& color)
	{
		const uint32_t maxLineVertexCount = Renderer2DData::MaxLines / 2;
		if (s_Data.LineVertexCount >= maxLineVertexCount)
		{
			NextBatch();
		}

		s_Data.LineVertexBufferPtr->Position = pos1;
		s_Data.LineVertexBufferPtr->Color = color;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineVertexBufferPtr->Position = pos2;
		s_Data.LineVertexBufferPtr->Color = color;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineVertexCount += 2;
	}

	void Renderer2D::DrawLine(const glm::vec2& pos1, const glm::vec2& pos2, const glm::vec4& color)
	{
		DrawLine({ pos1.x, pos1.y, 0.0f }, { pos2.x, pos2.y, 0.0f }, color);
	}

	void Renderer2D::DrawLine(const glm::vec2& pos, float deg, float length, const glm::vec4& color)
	{
		const float radians = glm::radians(deg);
		DrawLine(pos, { pos.x + length * cos(radians), pos.y + length * sin(radians) }, color);
	}

	void Renderer2D::DrawString(const std::string& string, const Ref<Font>& font, const glm::mat4& transform,
								const glm::vec4& color, float kerning, float lineSpacing,
								int entityID)
	{
		const auto& fontGeometry = font->GetMSDFData()->FontGeometry;
		const msdfgen::FontMetrics& metrics = fontGeometry.getMetrics();
		Ref<Texture2D> fontAtlas = font->GetAtlasTexture();

		s_Data.FontAtlasTexture = fontAtlas;

		double x = 0.0;
		double fsScale = 1.0 / (metrics.ascenderY - metrics.descenderY);
		double y = 0.0;

		const float spaceGlyphAdvance = float(fontGeometry.getGlyph(' ')->getAdvance());

		for (size_t i = 0; i < string.size(); i++)
		{
			char character = string[i];
			if (character == '\r')
				continue;

			if (character == '\n')
			{
				x = 0;
				y -= fsScale * metrics.lineHeight + lineSpacing;
				continue;
			}

			if (character == ' ')
			{
				float advance = spaceGlyphAdvance;
				if (i < string.size() - 1)
				{
					char nextCharacter = string[i + 1];
					double dAdvance;
					fontGeometry.getAdvance(dAdvance, character, nextCharacter);
					advance = (float)dAdvance;
				}

				x += fsScale * advance + kerning;
				continue;
			}

			if (character == '\t')
			{
				// NOTE(Yan): is this right?
				x += 4.0f * (fsScale * spaceGlyphAdvance + kerning);
				continue;
			}

			auto glyph = fontGeometry.getGlyph(character);
			if (!glyph)
				glyph = fontGeometry.getGlyph('?');
			if (!glyph)
				return;

			double al, ab, ar, at;
			glyph->getQuadAtlasBounds(al, ab, ar, at);
			glm::vec2 texCoordMin((float)al, (float)ab);
			glm::vec2 texCoordMax((float)ar, (float)at);

			double pl, pb, pr, pt;
			glyph->getQuadPlaneBounds(pl, pb, pr, pt);
			glm::vec2 quadMin((float)pl, (float)pb);
			glm::vec2 quadMax((float)pr, (float)pt);

			quadMin *= fsScale, quadMax *= fsScale;
			quadMin += glm::vec2(x, y);
			quadMax += glm::vec2(x, y);

			float texelWidth = 1.0f / fontAtlas->GetWidth();
			float texelHeight = 1.0f / fontAtlas->GetHeight();
			texCoordMin *= glm::vec2(texelWidth, texelHeight);
			texCoordMax *= glm::vec2(texelWidth, texelHeight);

			// render here
			s_Data.TextVertexBufferPtr->Position = transform * glm::vec4(quadMin, 0.0f, 1.0f);
			s_Data.TextVertexBufferPtr->Color = color;
			s_Data.TextVertexBufferPtr->TexCoord = texCoordMin;
			s_Data.TextVertexBufferPtr->EntityID = entityID;
			s_Data.TextVertexBufferPtr++;

			s_Data.TextVertexBufferPtr->Position = transform * glm::vec4(quadMin.x, quadMax.y, 0.0f, 1.0f);
			s_Data.TextVertexBufferPtr->Color = color;
			s_Data.TextVertexBufferPtr->TexCoord = { texCoordMin.x, texCoordMax.y };
			s_Data.TextVertexBufferPtr->EntityID = entityID;
			s_Data.TextVertexBufferPtr++;

			s_Data.TextVertexBufferPtr->Position = transform * glm::vec4(quadMax, 0.0f, 1.0f);
			s_Data.TextVertexBufferPtr->Color = color;
			s_Data.TextVertexBufferPtr->TexCoord = texCoordMax;
			s_Data.TextVertexBufferPtr->EntityID = entityID;
			s_Data.TextVertexBufferPtr++;

			s_Data.TextVertexBufferPtr->Position = transform * glm::vec4(quadMax.x, quadMin.y, 0.0f, 1.0f);
			s_Data.TextVertexBufferPtr->Color = color;
			s_Data.TextVertexBufferPtr->TexCoord = { texCoordMax.x, texCoordMin.y };
			s_Data.TextVertexBufferPtr->EntityID = entityID;
			s_Data.TextVertexBufferPtr++;

			s_Data.TextIndexCount += 6;
			// s_Data.Stats.QuadCount++;

			if (i < string.size() - 1)
			{
				double advance = glyph->getAdvance();
				char nextCharacter = string[i + 1];
				fontGeometry.getAdvance(advance, character, nextCharacter);

				x += fsScale * advance + kerning;
			}
		}
	}

	void Renderer2D::DrawString(const std::string& string, const Ref<Font>& font, const glm::vec3& position, const glm::vec2& size,
								const glm::vec4& color, float kerning, float lineSpacing,
								int entityID)
	{
		const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawString(string, font, transform, color, kerning, lineSpacing, entityID);
	}
}
