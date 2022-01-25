#include "Engine.h"
#include "EntryPoint.h"

#include "Application.h"
#include "Layer\EditorLayer.h"

namespace Engine
{

	class EditorApplication : public Application
	{
	public:
		EditorApplication()
			: Application("Editor")
		{
			AddOverlay(new Editor::EditorLayer());
		}
	};

	Application* Create(const ApplicationArgs& args)
	{
		return new EditorApplication();
	}
}
