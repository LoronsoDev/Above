#include <Above.h>
#include <Above/Core/EntryPoint.h>

#include "Platform/OpenGL/OpenGLShader.h"
#include "imgui/imgui.h"
#include "EditorLayer.h"

#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Above
{
	class AboveEditor : public Application
	{
	public:
		AboveEditor()
			: Application("ABOVE Editor")
		{
			//PushLayer(new ExampleLayer());
			PushLayer(new EditorLayer());
		}
		~AboveEditor() {}
	};

	Application* Above::CreateApplication()
	{
		AB_INFO("APPLICATION INITIALIZED");
	
		return new AboveEditor();
	}

}