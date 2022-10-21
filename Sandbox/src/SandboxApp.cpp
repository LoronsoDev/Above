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
		AB_TRACE("ExampleLayer::Update");
	}
	
	void OnEvent(Above::Event& e) override
	{
		AB_INFO("{0}", e);
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
