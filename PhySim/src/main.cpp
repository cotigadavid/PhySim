#include "pspch.h"

#include "Core/Application.h"

int main(int argc, char** argv)
{
	PhySim::Log::Init();

	auto app = new PhySim::Application();
	app->Run();
	delete app;
}