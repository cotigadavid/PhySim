#pragma once

#include "Core.h"
#include "Core/Log.h"
#include "Entities/Entity.h"
#include "Entities/Scene.h"

#include "Events/Event.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"

namespace PhySim {
	
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const std::shared_ptr<Scene>& scene);

		void SetContext(const std::shared_ptr<Scene>& scene);

		void OnImGuiRender();

		Entity* GetSelectedEntity() const 
		{
			if (m_SelectionIndex == -1)
				return nullptr;

			return m_Context->m_Entities[m_SelectionIndex]; 
		}
	
	private:
		void DrawEntityNode(int index);
		void DrawComponents(int index);

	private:
		std::shared_ptr<Scene> m_Context;
		int m_SelectionIndex;
	};
}