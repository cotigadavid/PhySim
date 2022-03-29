#include "pspch.h"
#include "Scene.h"

#include "OpenGL/Renderer2D.h"

#include "Entities/Entity.h"

#include "Core/Application.h"

#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_circle_shape.h"

#include "Core/Input.h"
#include "Core/KeyCodes.h"

namespace PhySim {

	static b2BodyType Rigidbody2DTypeToBox2DBody(Rigidbody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
			case Rigidbody2DComponent::BodyType::Static:    return b2_staticBody;
			case Rigidbody2DComponent::BodyType::Dynamic:   return b2_dynamicBody;
			case Rigidbody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
		}

		PS_ASSERT(false, "Unknown body type");
		return b2_staticBody;
	}

	Scene::Scene()
	{

	}

	Scene::~Scene()
	{

	}

	void Scene::CreteQuad(const std::string& name)
	{
		Entity* newEnt = new Entity(name, this);
		newEnt->spriteComponent = new SpriteComponent();
		AddEntity(newEnt);
	}

	void Scene::CreteCircle(const std::string& name)
	{
		Entity* newEnt = new Entity(name, this);
		newEnt->circleComponent = new CircleComponent();
		AddEntity(newEnt);
	}

	void Scene::CreteTriangle(const std::string& name)
	{
		Entity* newEnt = new Entity(name, this);
		newEnt->triangleComponent = new TriangleComponent();
		AddEntity(newEnt);
	}
	
	void Scene::DestroyEntity(Entity entity)
	{

	}

	void Scene::AddEntity(PhySim::Entity* entity)
	{
		entity->m_Scene = this;
		m_Entities.push_back(entity);
	}

	void Scene::DuplicateEntity(Entity* entity)
	{
		Entity* newEntity = new Entity(*entity);
		AddEntity(newEntity);
	}

	std::shared_ptr<Scene> Scene::CopyScene(std::shared_ptr<Scene> other)
	{
		std::shared_ptr<Scene> newScene = std::make_shared<Scene>();

		newScene->m_ViewportHeight = other->m_ViewportHeight;
		newScene->m_ViewportWidth = other->m_ViewportWidth;

		//newScene->m_PhysicsWorld = other->m_PhysicsWorld;

		//newScene->m_Entities = other->m_Entities;

		for (int i = 0; i < other->m_Entities.size(); ++i)
		{
			newScene->m_Entities.push_back(new Entity(*other->m_Entities[i]));
		}

		return newScene;
	}

	void Scene::OnRuntimeStart()
	{
		m_PhysicsWorld = new b2World({ 0.0f, -9.8f });

		//auto view = m_Registry.view<Rigidbody2DComponent>();
		for (auto entity : m_Entities)
		{
			//Entity entity = { e, this };
			//auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity->rb2d;

			if (rb2d)
			{
				b2BodyDef bodyDef;
				bodyDef.type = Rigidbody2DTypeToBox2DBody(rb2d->Type);
				bodyDef.position.Set(entity->m_Translation.x, entity->m_Translation.y);
				bodyDef.angle = entity->m_Rotation.z;

				b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
				body->SetFixedRotation(rb2d->FixedRotation);
				rb2d->RuntimeBody = body;

				if (entity->bc2d)
				{
					auto& bc2d = entity->bc2d;

					b2PolygonShape boxShape;
					boxShape.SetAsBox(bc2d->Size.x * entity->m_Scale.x, bc2d->Size.y * entity->m_Scale.y);
					
					b2FixtureDef fixtureDef;
					fixtureDef.shape = &boxShape;
					fixtureDef.density = bc2d->Density;
					fixtureDef.friction = bc2d->Friction;
					fixtureDef.restitution = bc2d->Restitution;
					fixtureDef.restitutionThreshold = bc2d->RestitutionThreshold;
					body->CreateFixture(&fixtureDef);
				}

				if (entity->cc2d)
				{
					auto& cc2d = entity->cc2d;

					b2CircleShape circleShape;
					circleShape.m_p.Set(cc2d->Offset.x, cc2d->Offset.y);
					circleShape.m_radius = entity->m_Scale.x * cc2d->Radius;

					b2FixtureDef fixtureDef;
					fixtureDef.shape = &circleShape;
					fixtureDef.density = cc2d->Density;
					fixtureDef.friction = cc2d->Friction;
					fixtureDef.restitution = cc2d->Restitution;
					fixtureDef.restitutionThreshold = cc2d->RestitutionThreshold;
					body->CreateFixture(&fixtureDef);
				}

				if (entity->tc2d)
				{
					auto& tc2d = entity->tc2d;

					b2Vec2 vertices[3];

					vertices[0].Set(-1 * entity->m_Scale.x / 2, -1 * entity->m_Scale.y / 2);
					vertices[1].Set(entity->m_Scale.x / 2, -1 * entity->m_Scale.y / 2);
					vertices[2].Set(-1 * entity->m_Scale.x / 2, entity->m_Scale.y / 2);
																						
					b2PolygonShape Shape;
					
					Shape.Set(vertices, 3);

					b2FixtureDef fixtureDef;
					fixtureDef.shape = &Shape;
					fixtureDef.density = tc2d->Density;
					fixtureDef.friction = tc2d->Friction;
					fixtureDef.restitution = tc2d->Restitution;
					fixtureDef.restitutionThreshold = tc2d->RestitutionThreshold;
					body->CreateFixture(&fixtureDef);
				}
			}
		}
	}

	void Scene::OnRuntimeStop()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		const int32_t velocityIterations = 6;
		const int32_t positionIterations = 2;
		m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);

		// Retrieve transform from Box2D
		for (auto entity : m_Entities)
		{
			auto& rb2d = entity->rb2d;

			if (rb2d)
			{
				b2Body* body = (b2Body*)rb2d->RuntimeBody;
				const auto& position = body->GetPosition();
				entity->m_Translation.x = position.x;
				entity->m_Translation.y = position.y;
				entity->m_Rotation.z = body->GetAngle();
			}
		}

		if (Input::IsKeyPressed(Key::M))
		{
			if (m_Entities[1]->rb2d)
				m_Entities[1]->rb2d->RuntimeBody->ApplyForce(b2Vec2(0, 50), m_Entities[1]->rb2d->RuntimeBody->GetWorldCenter(), 0);
		}

		OnUpdateEditor(ts);
	}

	void Scene::OnUpdateEditor(Timestep ts)
	{
		Renderer2D::BeginScene(Application::Get().GetProjectionData());		

	
		for (unsigned int index = 0; index < m_Entities.size(); ++index)
		{
			Entity* entity = m_Entities[index];
			if (entity->spriteComponent)
			{
				if (entity->spriteComponent->m_Texture)
					Renderer2D::DrawQuad(entity->GetTransform(), entity->spriteComponent->m_Texture, index, entity->spriteComponent->m_TilingFactor, entity->spriteComponent->m_Color);
				else
					Renderer2D::DrawQuad(entity->GetTransform(), entity->spriteComponent->m_Color, index);

			}
			if (entity->circleComponent)
			{
				Renderer2D::DrawCircle(entity->GetTransform(), entity->circleComponent->m_Color, entity->circleComponent->m_Thickness, entity->circleComponent->m_Fade, index);
			}
			if (entity->triangleComponent)
			{
				Renderer2D::DrawTriangle(entity->GetTransform(), entity->triangleComponent->m_Color, index);
			}
		}

		//flush
		Renderer2D::DrawQuad(glm::vec2(-20.0f), glm::vec2(1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		Renderer2D::DrawQuad(glm::vec2(-20.0f), glm::vec2(1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		Renderer2D::DrawQuad(glm::vec2(-20.0f), glm::vec2(1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		Renderer2D::DrawQuad(glm::vec2(-20.0f), glm::vec2(1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		Renderer2D::DrawQuad(glm::vec2(-20.0f), glm::vec2(1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		Renderer2D::DrawQuad(glm::vec2(-20.0f), glm::vec2(1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		Renderer2D::DrawQuad(glm::vec2(-20.0f), glm::vec2(1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		Renderer2D::DrawQuad(glm::vec2(-20.0f), glm::vec2(1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		Renderer2D::DrawQuad(glm::vec2(-20.0f), glm::vec2(1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		

		Renderer2D::EndScene();
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;
	}
}