#include <Above.h>

class Sandbox : public Above::Application
{
public:
	Sandbox() {}
	~Sandbox() {}
};

Above::Application* Above::CreateApplication()
{
	return new Sandbox;
}
