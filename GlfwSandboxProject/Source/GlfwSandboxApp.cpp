#include "Engine.h"
#include "EntryPoint.h"

#include "GlfwGame.h"

namespace Engine
{
	class GlfwSandboxApp : public Application
	{
	public:
		GlfwSandboxApp()
			: Application("GlfwSandboxApp",
				Engine::FileUtils::GetWorkingDirectory())
		{
			AddLayer(new GlfwSandbox::GlfwGame());
		}
	};

	Application* Create(const Engine::ApplicationArgs& args)
	{
		return new GlfwSandboxApp();
	}
}
