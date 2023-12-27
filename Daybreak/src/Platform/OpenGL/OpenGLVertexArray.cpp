#include "dbpch.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

#include "glad/glad.h"

namespace Daybreak
{
	static GLenum RenderDataTypesToOpenGLBaseType(RenderDataTypes type)
	{
		switch (type)
		{
			case RenderDataTypes::Float:    return GL_FLOAT;
			case RenderDataTypes::Float2:   return GL_FLOAT;
			case RenderDataTypes::Float3:   return GL_FLOAT;
			case RenderDataTypes::Float4:   return GL_FLOAT;
			case RenderDataTypes::Mat3:     return GL_FLOAT;
			case RenderDataTypes::Mat4:     return GL_FLOAT;
			case RenderDataTypes::Int:      return GL_INT;
			case RenderDataTypes::Int2:     return GL_INT;
			case RenderDataTypes::Int3:     return GL_INT;
			case RenderDataTypes::Int4:     return GL_INT;
			case RenderDataTypes::Bool:     return GL_BOOL;
		}
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		DB_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");


		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			switch (element.Type)
			{
			case RenderDataTypes::Float:
			case RenderDataTypes::Float2:
			case RenderDataTypes::Float3:
			case RenderDataTypes::Float4:
			{
				glEnableVertexAttribArray(m_VertexBufferIndex);
				glVertexAttribPointer(m_VertexBufferIndex,
					element.GetComponentCount(),
					RenderDataTypesToOpenGLBaseType(element.Type),
					element.Normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)element.Offset);
				m_VertexBufferIndex++;
				break;
			}
			case RenderDataTypes::Int:
			case RenderDataTypes::Int2:
			case RenderDataTypes::Int3:
			case RenderDataTypes::Int4:
			case RenderDataTypes::Bool:
			{
				glEnableVertexAttribArray(m_VertexBufferIndex);
				glVertexAttribIPointer(m_VertexBufferIndex,
					element.GetComponentCount(),
					RenderDataTypesToOpenGLBaseType(element.Type),
					layout.GetStride(),
					(const void*)element.Offset);
				m_VertexBufferIndex++;
				break;
			}
			case RenderDataTypes::Mat3:
			case RenderDataTypes::Mat4:
			{
				uint8_t count = element.GetComponentCount();
				for (uint8_t i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(m_VertexBufferIndex);
					glVertexAttribPointer(m_VertexBufferIndex,
						count,
						RenderDataTypesToOpenGLBaseType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)(element.Offset + sizeof(float) * count * i));
					glVertexAttribDivisor(m_VertexBufferIndex, 1);
					m_VertexBufferIndex++;
				}
				break;
			}
			default:
				DB_CORE_ASSERT(false, "Unknown RenderDataTypes!");
			}
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}
}