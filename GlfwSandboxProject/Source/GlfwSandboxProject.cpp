// GlfwSandboxProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "Application.h"
#include "GlfwGame.h"
#include "EditorLayer.h"

int main()
{
	Engine::Application application("Glfw Sandbox");
	application.AddLayer(new GlfwSandbox::GlfwGame());
	application.Run();
	return 0;
}