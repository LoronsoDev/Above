#include "abpch.h"

#include <Above.h>

class ExampleLayer : public Above::Layer
{
public:
	ExampleLayer() 
		: Layer("Example")
	{

	}

	void OnUpdate() override
	{
		if (Above::Input::IsKeyPressed(AB_KEY_TAB))
			AB_INFO("TAB KEY");
	}
	
	void OnEvent(Above::Event& e) override
	{
		
	}
};

class Sandbox : public Above::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox() {}
};

Above::Application* Above::CreateApplication()
{
	AB_INFO("APPLICATION INITIALIZED");
	
	return new Sandbox;
}
