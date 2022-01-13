#pragma once

#include "Entities/Scene.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace PhySim {

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

		Scene* m_Scene = nullptr;

		bool m_HasSpriteRendererComponent = false;
	};

	class Quad : public Entity
	{
	public:
		Quad(const std::string& name, Scene* scene);
		Quad(const std::string& name, const glm::vec3& translation, Scene* scene);
		Quad(const std::string& name, const glm::vec3& translation, const glm::vec4& color, Scene* scene);
		Quad(const std::string& name, const glm::vec4& color, Scene* scene);
		virtual ~Quad() {}

		glm::vec4 m_Color{ 1.0f, 1.0f, 1.0f, 1.0f };
	};

	class Circle : public Entity
	{

	};
}

//PROBLEMS WITH ENTITY POSITION AND CALCULATE ENTITY TRANSFORM