#pragma once
#include "Above/Core/Layer.h"
#include "Above/Events/Event.h"

#include "Above/Events/ApplicationEvent.h"
#include "Above/Events/KeyEvent.h" 
#include "Above/Events/MouseEvent.h" 

namespace Above
{
	class ABOVE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }

	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};
}