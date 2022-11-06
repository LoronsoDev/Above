#pragma once
#include "Above/Layer.h"
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
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	private:
		float m_Time = 0.0f;
	};
}