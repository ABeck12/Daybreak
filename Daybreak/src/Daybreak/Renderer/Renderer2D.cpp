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
		//float TilingFactor;

		// Editor-only
		//int EntityID;
	};

	struct Renderer2DData
	{
		static const uint32_t MaxTextureSlots = 32;
		static const uint32_t MaxQuads = 10000;
		static const uint32_t MaxVertices = 4 * MaxQuads;
		static const uint32_t MaxIndices = 6 * MaxQuads;

		glm::mat4 ViewProjectionMatrix;

		// For Quads
		Ref<VertexBuffer> quadVb;
		//Ref<IndexBuffer> quadIb;
		Ref<VertexArray> quadVa;
		Ref<Shader> quadShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t quadIndexCount = 0;

		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		glm::vec4 QuadVertexPositions[4];

		uint32_t TextureSlotIndex = 1;
		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		s_Data.quadShader = Shader::Create("Renderer2D_QuadShader", "../Sandbox/assets/Renderer2D_QuadShader.glsl");

		s_Data.quadVa = VertexArray::Create();
		s_Data.quadVb = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		s_Data.quadVb->SetLayout({
				{ RenderDataTypes::Float3, std::string("a_Position") },
				{ RenderDataTypes::Float2, std::string("a_TexCoord") },
				{ RenderDataTypes::Float4, std::string("a_Color") },
				{ RenderDataTypes::Float, std::string("a_TexIndex") },
				//{ RenderDataTypes::Float, std::string("a_TilingFactor") },
			});
		s_Data.quadVa->AddVertexBuffer(s_Data.quadVb);

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
		s_Data.quadVa->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		s_Data.WhiteTexture = Texture2D::Create({ 1, 1, Daybreak::ImageFormat::RGBA, Daybreak::TextureFilterType::Point });
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		//s_Data.WhiteTexture = Texture2D::Create({ 1, 1, Daybreak::ImageFormat::RGBA, Daybreak::TextureFilterType::Point }, "../Sandbox/assets/WhitePixel.png");

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		s_Data.quadShader->Bind();
		s_Data.quadShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

	}

	void Renderer2D::Shutdown()
	{
		delete[] s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		s_Data.ViewProjectionMatrix = camera.GetProjection() * transform;
		s_Data.quadShader->Bind();

		s_Data.quadShader->SetMat4("u_ViewProjection", s_Data.ViewProjectionMatrix);

		s_Data.quadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;

		StartBatch();
	}

	void Renderer2D::EndScene()
	{
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
		s_Data.quadVb->SetData(s_Data.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);

		RenderCommand::DrawIndexed(s_Data.quadVa, s_Data.quadIndexCount);
	}

	void Renderer2D::StartBatch()
	{
		s_Data.quadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& tintColor)
	{
		DrawQuad(transform, tintColor, s_Data.WhiteTexture);
	}

	// This is the main DrawQuad function for now
	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& tintColor, const Ref<Texture2D>& texture)
	{
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (s_Data.quadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
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
		//float tilingFactor = 0.0f;
		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			//s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			//s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}
		s_Data.quadIndexCount += 6;

	}
}