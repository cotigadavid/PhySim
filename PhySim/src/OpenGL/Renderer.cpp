#include "pspch.h"
#include "Renderer.h"

#include "Renderer2D.h"
#include <glad/glad.h>

namespace PhySim {
	void Renderer::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);
		Renderer2D::Init();
	}

	void Renderer::BeginScene()
	{
		
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_Transform", transform);
		vertexArray->Bind();
		DrawIndexed(vertexArray);
	}

	void Renderer::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void Renderer::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount)
	{
		vertexArray->Bind();
		uint32_t count;
		if (vertexArray->GetIndexBuffer())
			count = indexCount ? vertexArray->GetIndexBuffer()->GetCount() : indexCount;
		else
			count = indexCount;
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Renderer::DrawLines(const std::shared_ptr<VertexArray>& vertexArray, uint32_t vertexCount)
	{
		vertexArray->Bind();
		glDrawArrays(GL_LINES, 0, vertexCount);
	}

	void Renderer::SetLineWidth(float width)
	{
		PS_TRACE("{0}", width);
		glLineWidth(width);
	}
}

//Try to compile Hazel by modifing shaders // no vulkan