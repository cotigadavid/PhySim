#pragma once

#include "SceneHierarchyPanel.h"

#include "Entities/Scene.h"

namespace PhySim {

	class PhysicsPanel
	{
	public:
		PhysicsPanel();

		void OnImGuiRender();

		void SetSceneHierarchyPanel(std::shared_ptr<SceneHierarchyPanel> SceneHierarchyPanel) { m_SceneHierarchyPanel = SceneHierarchyPanel; }
		std::shared_ptr<SceneHierarchyPanel> GetSceneHierarchyPanel() { return m_SceneHierarchyPanel; }

		std::shared_ptr<Scene> m_Context;

	private:
		std::shared_ptr<SceneHierarchyPanel> m_SceneHierarchyPanel;
	};
}