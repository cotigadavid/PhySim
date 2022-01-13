#pragma once

#include <vector>
#include "Core/Timestep.h"

//#include "Entities/Entity.h"

namespace PhySim
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		void CreteQuad(const std::string& name);
		void CreteCircle(const std::string& name);

		void DestroyEntity(Entity entity);

		void AddEntity(Entity* entity);

		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);

	
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		std::vector<Entity*> m_Entities;
	};
}