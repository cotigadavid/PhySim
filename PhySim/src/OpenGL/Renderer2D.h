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

		static void SetLineWidth(float width);
		static float GetLineWidth();

		static void DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int index = -1);
		static void DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int index = -1);
		static void DrawRect(const glm::mat4& transform, const glm::vec4& color, int index = -1);

		static void DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness = 1, float fade = 0.005, int index = -1);

		static void DrawTriangle(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec4& color, int index = -1);
		static void DrawTriangle(const glm::mat4& transform, const glm::vec4& color, int index = -1);

		static void DrawQuad(const Entity& entity);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, int index = -1);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int index = -1);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), int index = -1);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), int index = -1);

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int index = -1);
		static void DrawQuad(const glm::mat4& transform, const std::shared_ptr<Texture>& texture, int index = -1, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color, int index = -1);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color, int index = -1);
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const std::shared_ptr<Texture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), int index = -1);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const std::shared_ptr<Texture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), int index = -1);
	
	private:
		static void StartBatch();
		static void NextBatch();
	};
}