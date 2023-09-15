#pragma once

#include <Above/Core/Timestep.h>

#include <Above/Scene/Components.h>
#include <Above/Renderer/Renderer2D.h>

#include <glm/glm.hpp>

#include <entt.hpp>

namespace Above
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		entt::entity CreateEntity();

		//TEMP
		entt::registry& Reg() { return m_Registry; }

		void OnUpdate(Timestep ts);
	private:
		entt::registry m_Registry; //entity IDs and component data
	};
}