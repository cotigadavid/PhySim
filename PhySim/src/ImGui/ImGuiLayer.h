#pragma once

#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#include "Core/Layer.h"

//#include "Core/Application.h"
//#include <fstream>

//struct CarData::ResultData;

namespace PhySim {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		//void OnAttach();
		virtual void OnEvent(Event& e) override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	private:
		/*bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnKeyTypedEvent(KeyTypedEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);*/

		void SetDarkThemeColors();

	private:
		//ResultData layerResultData;
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};
}