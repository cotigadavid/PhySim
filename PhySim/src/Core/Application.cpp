#include "pspch.h"

#include "Application.h"
#include "OpenGL/Renderer.h"
#include "OpenGL/Renderer2D.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Editor/EditorLayer.h"

namespace PhySim {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
		: m_ProjectionData(-10.0f, 10, -10.0f, 10.0f)
	{
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		Renderer::Init();

		m_ImguiLayer = new ImGuiLayer();
		m_ImguiLayer->OnAttach();


		Rigidbody2DComponent();
		m_Scene = std::make_shared<Scene>();

		EditorLayer* editorLayer = new EditorLayer();

		m_LayerStack.PushLayer(editorLayer);
		editorLayer->OnAttach();

		Entity* quad2 = new Entity("quad2");
		quad2->spriteComponent = new SpriteComponent();
		quad2->m_Translation = { 3.5f, 3.5f, 1.0f };
		quad2->spriteComponent->m_Color = { 0.5f, 1.0f, 0.5f, 1.0f };
		m_Scene->AddEntity(quad2);

		Entity* quad3 = new Entity("quad3");
		quad3->spriteComponent = new SpriteComponent();
		quad3->m_Translation = { 5.3f, 5.3f, 1.0f };
		quad3->spriteComponent->m_Color = { 1.0f, 0.5f, 0.5f, 1.0f };
		m_Scene->AddEntity(quad3);

		Entity* circle1 = new Entity("circle1");
		circle1->circleComponent = new CircleComponent();
		circle1->m_Translation = { 5.3f, 5.3f, 1.0f };
		circle1->circleComponent->m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		m_Scene->AddEntity(circle1);
	}

	Application::~Application()
	{
		
	}
	
	void Application::Run()
	{
		while (m_Running)
		{
			m_ProjectionData.RecalculateViewMatrix();

			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(timestep);

			m_ImguiLayer->Begin();

			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();

			m_ImguiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::OnUpdate(Timestep ts)
	{
		
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClosed));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

		for (std::vector<Layer*>::iterator it = m_LayerStack.begin(); it != m_LayerStack.end(); ++it)
		{
			if (e.Handled)
				break;
			(*it)->OnEvent(e);
		}
	}

	void Application::OnResize(float width, float height)
	{
		glViewport(0, 0, width, height);

		float aspectRatio = width / height;
		m_ProjectionData.SetProjection(-10.0f * aspectRatio, 10.0f * aspectRatio, -10.0f , 10.0f );
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		
		OnResize((float)e.GetWidth(), (float)e.GetHeight());

		return false;
	}

}