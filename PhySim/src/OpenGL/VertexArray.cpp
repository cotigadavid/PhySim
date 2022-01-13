#include "pspch.h"
#include "VertexArray.h"

#include <glad/glad.h>

namespace PhySim {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case PhySim::ShaderDataType::Float:    return GL_FLOAT;
			case PhySim::ShaderDataType::Float2:   return GL_FLOAT;
			case PhySim::ShaderDataType::Float3:   return GL_FLOAT;
			case PhySim::ShaderDataType::Float4:   return GL_FLOAT;
			case PhySim::ShaderDataType::Mat3:     return GL_FLOAT;
			case PhySim::ShaderDataType::Mat4:     return GL_FLOAT;
			case PhySim::ShaderDataType::Int:      return GL_INT;
			case PhySim::ShaderDataType::Int2:     return GL_INT;
			case PhySim::ShaderDataType::Int3:     return GL_INT;
			case PhySim::ShaderDataType::Int4:     return GL_INT;
			case PhySim::ShaderDataType::Bool:     return GL_BOOL;
		}

		PS_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	VertexArray::VertexArray() 
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void VertexArray::UnBind() const
	{
		glBindVertexArray(0);
	}

	void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		PS_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");
	
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		unsigned int index = 0;
		const auto& layout = vertexBuffer->GetLayout();

		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			index++;
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}