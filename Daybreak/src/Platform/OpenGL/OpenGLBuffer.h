#pragma once

#include "dbpch.h"

#include "Daybreak/Renderer/Buffer.h"

namespace Daybreak
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(const void* vertices, unsigned int size);
		OpenGLVertexBuffer(unsigned int size);

		virtual ~OpenGLVertexBuffer() = default;

		//virtual void Bind() override;
		//virtual void Unbind() override;
		// 
		//virtual void SetData(const void* data, uint32_t size) override;

	private:
		uint32_t m_RendererID;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(const void* indeces, unsigned int size);
		OpenGLIndexBuffer(unsigned int size);

		virtual ~OpenGLIndexBuffer() = default;

		//virtual void Bind() override;
		//virtual void Unbind() override;

		//virtual void SetData(const void* data, uint32_t size) override;

	private:
		uint32_t m_RendererID;
	};
}