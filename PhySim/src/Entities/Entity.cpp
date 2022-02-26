#include "pspch.h"
#include "Entity.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace PhySim {

	Entity::Entity(const std::string& name, Scene* scene)
		: m_Name(name), m_Scene(scene)
	{
		rb2d = new Rigidbody2DComponent();
		bc2d = new BoxCollider2DComponent();
	}

	Entity::Entity(const std::string& name, const glm::vec3& translation, Scene* scene)
		: m_Name(name), m_Translation(translation), m_Scene(scene)
	{
		rb2d = new Rigidbody2DComponent();
		bc2d = new BoxCollider2DComponent();
	}

	Quad::Quad(const Quad& quad)
	{
		m_Name = quad.m_Name;
		m_Scene = quad.m_Scene;
		m_Translation = quad.m_Translation;
		m_Rotation = quad.m_Rotation;
		m_Scale = quad.m_Scale;
		m_Color = quad.m_Color;
		m_HasSpriteRendererComponent = quad.m_HasSpriteRendererComponent;
	}

	Quad::Quad(const std::string& name)
	{
		m_Name = name;

		m_HasSpriteRendererComponent = true;
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