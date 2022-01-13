#include "pspch.h"
#include "Scene.h"

#include "OpenGL/Renderer2D.h"

#include "Entities/Entity.h"

namespace PhySim {

	Scene::Scene()
	{

	}

	Scene::~Scene()
	{

	}

	void Scene::CreteQuad(const std::string& name)
	{
		AddEntity(new Quad(name, this));
	}

	void Scene::CreteCircle(const std::string& name)
	{
		//AddEntity(new Circle(name, std::make_shared<Scene>(this)));
	}
	
	void Scene::DestroyEntity(Entity entity)
	{

	}

	void Scene::AddEntity(PhySim::Entity* entity)
	{
		m_Entities.push_back(entity);
	}

	void Scene::OnUpdate(Timestep ts)
	{
		for (Entity* entity : m_Entities)
		{
			Quad* quad = dynamic_cast<Quad*>(entity);
			if (quad)
			{
				Renderer2D::DrawQuad(quad->GetTransform(), quad->m_Color);

				PS_ERROR("{0}", quad->m_Name);
			}
		}

		//PS_ERROR("flushing");
		//flush
		Renderer2D::DrawQuad(glm::mat4(1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		Renderer2D::DrawQuad(glm::mat4(1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		Renderer2D::DrawQuad(glm::mat4(1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		Renderer2D::DrawQuad(glm::mat4(1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		Renderer2D::DrawQuad(glm::mat4(1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		Renderer2D::DrawQuad(glm::mat4(1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;
	}
}