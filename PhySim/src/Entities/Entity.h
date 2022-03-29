#pragma once

#include "Entities/Scene.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "OpenGL/Texture.h"

#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_circle_shape.h"

namespace PhySim {

	struct Rigidbody2DComponent
	{
		enum class BodyType { Static = 0, Dynamic, Kinematic };
		BodyType Type = BodyType::Static;
		bool FixedRotation = false;

		float VerticalForce = 1500.0f;
		float HorizontalForce = 0.0f;
		float VerticalImpulse = 0.0f;
		float HorizontalImpulse = 0.0f;

		b2Body* RuntimeBody = nullptr;

		Rigidbody2DComponent() = default;
		Rigidbody2DComponent(const Rigidbody2DComponent& other)
		{
			RuntimeBody = other.RuntimeBody;
			Type = other.Type;
			FixedRotation = other.FixedRotation;
		}
	};

	struct BoxCollider2DComponent
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size = { 0.5f, 0.5f };

		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		void* RuntimeFixture = nullptr;

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent& other)
		{
			Offset = other.Offset;
			Size = other.Size;

			Density = other.Density;
			Friction = other.Friction;
			Restitution = other.Restitution;
			RestitutionThreshold = other.RestitutionThreshold;
			RuntimeFixture = other.RuntimeFixture;
		}
	};

	struct CircleCollider2DComponent
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		float Radius = 0.5f;

		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		void* RuntimeFixture = nullptr;

		CircleCollider2DComponent() = default;
		CircleCollider2DComponent(const CircleCollider2DComponent& other)
		{
			Offset = other.Offset;
			Radius = other.Radius;

			Density = other.Density;
			Friction = other.Friction;
			Restitution = other.Restitution;
			RestitutionThreshold = other.RestitutionThreshold;
			RuntimeFixture = other.RuntimeFixture;
		}
	};

	struct TriangleCollider2DComponent
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size = { 0.5f, 0.5f };

		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		void* RuntimeFixture = nullptr;

		TriangleCollider2DComponent() = default;
		TriangleCollider2DComponent(const TriangleCollider2DComponent& other)
		{
			Offset = other.Offset;
			Size = other.Size;

			Density = other.Density;
			Friction = other.Friction;
			Restitution = other.Restitution;
			RestitutionThreshold = other.RestitutionThreshold;
			RuntimeFixture = other.RuntimeFixture;
		}
	};

	class SpriteComponent
	{
	public:

		glm::vec4 m_Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		std::shared_ptr<Texture> m_Texture = nullptr;
		float m_TilingFactor = 1;

		SpriteComponent() = default;
		SpriteComponent(const SpriteComponent& other)
		{
			m_Color = other.m_Color;
			m_Texture = other.m_Texture;
			m_TilingFactor = other.m_TilingFactor;
		}
	};

	struct CircleComponent
	{
		glm::vec4 m_Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		float m_Thickness = 1.0f;
		float m_Fade = 0.005f;

		CircleComponent() = default;
		CircleComponent(const CircleComponent& other)
		{
			m_Color = other.m_Color;
			m_Thickness = other.m_Thickness;
			m_Fade = other.m_Fade;
		}
	};

	class TriangleComponent
	{
	public:

		glm::vec4 m_Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		float m_TilingFactor = 1;

		TriangleComponent() = default;
		TriangleComponent(const TriangleComponent& other)
		{
			m_Color = other.m_Color;
			m_TilingFactor = other.m_TilingFactor;
		}
	};

	class Entity
	{
	public:
		Entity() = default;
		Entity(const Entity& entity);
		Entity(const std::string& name, Scene* scene = nullptr);
		Entity(const std::string& name, const glm::vec3& translation, Scene* scene);
		Entity(const std::string& name, const glm::vec3& translation, const glm::vec4& color, Scene* scene);
		Entity(const std::string& name, const glm::vec4& color, Scene* scene);
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
		
		void InitializePhysiscs();

		glm::vec3 m_Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Scale = { 1.0f, 1.0f, 1.0f };
		std::string m_Name;

		Rigidbody2DComponent* rb2d = nullptr;
		BoxCollider2DComponent* bc2d = nullptr;
		CircleCollider2DComponent* cc2d = nullptr;
		TriangleCollider2DComponent* tc2d = nullptr;
		SpriteComponent* spriteComponent = nullptr;
		CircleComponent* circleComponent = nullptr;
		TriangleComponent* triangleComponent = nullptr;

		Scene* m_Scene = nullptr;
	};

	/*class Quad : public Entity
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
	};*/

	/*class Circle : public Entity
	{

	};*/

}
