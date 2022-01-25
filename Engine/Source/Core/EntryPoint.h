#pragma once

#include "Application.h"

extern Engine::Application* Engine::Create(const Engine::ApplicationArgs& args);

int main(int argsc, char** argsv)
{
	Engine::Application* application = Engine::Create({ argsc, argsv });
	application->Run();
	delete application;
	return 0;
}