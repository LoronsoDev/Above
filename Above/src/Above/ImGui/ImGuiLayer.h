#pragma once
#include "Above/Layer.h"
#include "Above/Events/Event.h"

namespace Above
{
	class ABOVE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& event);

	private:
		float m_Time = 0.0f;
	};
}