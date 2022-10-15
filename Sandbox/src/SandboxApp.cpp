#include "abpch.h"

#include <Above.h>

class Sandbox : public Above::Application
{
public:
	Sandbox() {}
	~Sandbox() {}
};

Above::Application* Above::CreateApplication()
{
	AB_INFO("APPLICATION INITIALIZED");
	
	return new Sandbox;
}
