#pragma once

#include <stdint.h>
#include <string>
#include <vector>

#include "Daybreak/Core/Base.h"

namespace Daybreak
{
	enum class RenderDataTypes
	{
		Float,
		Float2,
		Float3,
		Float4,
		Mat3,
		Mat4,
		Int,
		Int2,
		Int3,
		Int4,
		Bool,
	};

	static uint32_t RenderDataTypeSize(RenderDataTypes type)
	{
		switch (type)
		{
			case RenderDataTypes::Float: return 4; break;
			case RenderDataTypes::Float2: return 4 * 2; break;
			case RenderDataTypes::Float3: return 4 * 3; break;
			case RenderDataTypes::Float4: return 4 * 4; break;
			case RenderDataTypes::Mat3: return 4 * 3 * 3; break;
			case RenderDataTypes::Mat4: return 4 * 4 * 4; break;
			case RenderDataTypes::Int: return 4; break;
			case RenderDataTypes::Int2: return 4 * 2; break;
			case RenderDataTypes::Int3: return 4 * 3; break;
			case RenderDataTypes::Int4: return 4 * 4; break;
			case RenderDataTypes::Bool: return 1; break;
		}
		return 0;
	}

	struct BufferElement
	{
		BufferElement() = default;
		BufferElement(RenderDataTypes type, std::string name, bool normalized = false)
			: Type(type), Name(name), Normalized(normalized), Offset(0), Size(RenderDataTypeSize(type)) {}

		RenderDataTypes Type;
		bool Normalized;
		std::string Name;
		uint32_t Size;
		size_t Offset;

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
				case RenderDataTypes::Float: return 1; break;
				case RenderDataTypes::Float2: return 2; break;
				case RenderDataTypes::Float3: return 3; break;
				case RenderDataTypes::Float4: return 4; break;
				case RenderDataTypes::Mat3: return 3; break;
				case RenderDataTypes::Mat4: return 4; break;
				case RenderDataTypes::Int: return 1; break;
				case RenderDataTypes::Int2: return 2; break;
				case RenderDataTypes::Int3: return 3; break;
				case RenderDataTypes::Int4: return 4; break;
				case RenderDataTypes::Bool: return 1; break;
			}
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() {}
		BufferLayout(std::initializer_list<BufferElement> elements)
			: m_Elements(elements)
		{
			CalculateOffsetsAndStride();
		}

		const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		uint32_t GetStride() const { return m_Stride; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		void CalculateOffsetsAndStride()
		{
			size_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}

	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		static Ref<VertexBuffer> Create(uint32_t size);
		static Ref<VertexBuffer> Create(const float* vertices, uint32_t size);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		static Ref<IndexBuffer> Create(const uint32_t* indices, uint32_t count);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;
	};
}