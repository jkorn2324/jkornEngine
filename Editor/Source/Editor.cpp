#include "EditorPCH.h"
#include "Layer\EditorLayer.h"

int main()
{
	Engine::Application application("Editor");
	application.AddOverlay(new Editor::EditorLayer());
	application.Run();
	return 0;
}