#pragma once

#include <glm/glm.hpp>

#include "Core/ProjectionData.h"
#include "Texture.h"

#include "Entities/Entity.h"

namespace PhySim {

	class Renderer2D
	{
	public:
		static void Init();
		static void ShutDown();

		static void BeginScene(ProjectionData& projectionData);
		static void EndScene();

		static void Flush();

		static void DrawQuad(const Quad& quad);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& transform, const std::shared_ptr<Texture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const std::shared_ptr<Texture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const std::shared_ptr<Texture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
	
	private:
		static void FlushAndReset();
	};
}