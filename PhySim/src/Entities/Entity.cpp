#include "pspch.h"
#include "Entity.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace PhySim {

	Entity::Entity(const std::string& name, Scene* scene)
		: m_Name(name), m_Scene(scene)
	{
	}

	Entity::Entity(const std::string& name, const glm::vec3& translation, Scene* scene)
		: m_Name(name), m_Translation(translation), m_Scene(scene)
	{
		
	}

	Quad::Quad(const std::string& name, Scene* scene)
	{
		m_Name = name;
		m_Scene = scene;

		m_HasSpriteRendererComponent = true;
	}

	Quad::Quad(const std::string& name, const glm::vec3& translation, Scene* scene)
	{
		m_Name = name;
		m_Translation = translation;
		m_Scene = scene;

		m_HasSpriteRendererComponent = true;
	}

	Quad::Quad(const std::string& name, const glm::vec3& translation, const glm::vec4& color, Scene* scene)
	{
		m_Name = name;
		m_Translation = translation;
		m_Color = color;
		m_Scene = scene;

		m_HasSpriteRendererComponent = true;
	}

	Quad::Quad(const std::string& name, const glm::vec4& color, Scene* scene)
		: m_Color(color)
	{
		m_Name = name;
		m_Scene = scene;

		m_HasSpriteRendererComponent = true;
	}

}