#pragma once

#include "Entities/Scene.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "OpenGL/Texture.h"

namespace PhySim {

	struct Rigidbody2DComponent
	{
		enum class BodyType { Static = 0, Dynamic, Kinematic };
		BodyType Type = BodyType::Static;
		bool FixedRotation = false;

		void* RuntimeBody = nullptr;

		Rigidbody2DComponent() = default;
		Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
	};

	struct BoxCollider2DComponent
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size = { 10.0f, 10.0f };

		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		void* RuntimeFixture = nullptr;

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
	};

	class Entity
	{
	public:
		Entity() {}
		Entity(const std::string& name, Scene* scene);
		Entity(const std::string& name, const glm::vec3& translation, Scene* scene);
		virtual ~Entity() {}

		bool operator==(const Entity& other) const
		{
			return this == &other;
			return m_Name == other.m_Name && m_Scene == other.m_Scene;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), 0.0f, { 1, 0, 0 })
				* glm::rotate(glm::mat4(1.0f), 0.0f, { 0, 1, 0 })
				* glm::rotate(glm::mat4(1.0f), m_Rotation.z, { 0, 0, 1 });

			return glm::translate(glm::mat4(1.0f), m_Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), m_Scale);
		}
		
		glm::vec3 m_Translation = { 100.0f, 50.0f, 0.0f };
		glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Scale = { 30.0f, 30.0f, 30.0f };
		std::string m_Name;

		Rigidbody2DComponent* rb2d = nullptr;
		BoxCollider2DComponent* bc2d = nullptr;

		Scene* m_Scene = nullptr;

		bool m_HasSpriteRendererComponent = false;
	};

	class Quad : public Entity
	{
	public:
		Quad(const Quad& quad);
		Quad(const std::string& name);
		Quad(const std::string& name, Scene* scene);
		Quad(const std::string& name, const glm::vec3& translation, Scene* scene);
		Quad(const std::string& name, const glm::vec3& translation, const glm::vec4& color, Scene* scene);
		Quad(const std::string& name, const glm::vec4& color, Scene* scene);
		virtual ~Quad() {}

		glm::vec4 m_Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		std::shared_ptr<Texture> m_Texture = nullptr;
		float m_TilingFactor = 1;
	};

	class Circle : public Entity
	{

	};

}
