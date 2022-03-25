#include "pspch.h"
#include "Entity.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace PhySim {

	Entity::Entity(const std::string& name, Scene* scene)
		: m_Name(name), m_Scene(scene)
	{
		InitializePhysiscs();
		//spriteComponent = new SpriteComponent();
	}

	Entity::Entity(const std::string& name, const glm::vec3& translation, Scene* scene)
		: m_Name(name), m_Translation(translation), m_Scene(scene)
	{
		InitializePhysiscs();
		//spriteComponent = new SpriteComponent();
	}

	Entity::Entity(const Entity& other)
	{
		m_Translation = other.m_Translation;
		m_Rotation = other.m_Rotation;
		m_Scale = other.m_Scale;
		m_Name = other.m_Name;

		if (other.rb2d) rb2d = new Rigidbody2DComponent(*other.rb2d);
		if (other.bc2d) bc2d = new BoxCollider2DComponent(*other.bc2d);
		if (other.cc2d) cc2d = new CircleCollider2DComponent(*other.cc2d);
		if (other.tc2d) tc2d = new TriangleCollider2DComponent(*other.tc2d);
		if (other.spriteComponent) spriteComponent = new SpriteComponent(*other.spriteComponent);
		if (other.circleComponent) circleComponent = new CircleComponent(*other.circleComponent);
		if (other.triangleComponent) triangleComponent = new TriangleComponent(*other.triangleComponent);

		m_Scene = other.m_Scene;
	}

	Entity::Entity(const std::string& name, const glm::vec3& translation, const glm::vec4& color, Scene* scene)
	{
		m_Name = name;
		m_Translation = translation;
		m_Scene = scene;

		spriteComponent = new SpriteComponent();
		spriteComponent->m_Color = color;

	}

	Entity::Entity(const std::string& name, const glm::vec4& color, Scene* scene)
	{
		m_Name = name;
		m_Scene = scene;

		spriteComponent = new SpriteComponent();
		spriteComponent->m_Color = color;
	}

	void Entity::InitializePhysiscs()
	{
		rb2d = new Rigidbody2DComponent();

		if (spriteComponent)
		{
			bc2d = new BoxCollider2DComponent();
			bc2d->Size = { m_Scale.x / 2, m_Scale.y / 2 };
		}
		if (circleComponent)
		{
			cc2d = new CircleCollider2DComponent();
			cc2d->Radius = m_Scale.x / 2;
		}
		if (triangleComponent)
		{
			tc2d = new TriangleCollider2DComponent();
			tc2d->Size = { m_Scale.x / 2, m_Scale.y / 2 };
		}
	}

}