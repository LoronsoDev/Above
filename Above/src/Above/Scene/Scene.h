#pragma once

#include <Above/Core/Timestep.h>

#include <Above/Scene/Components.h>
#include <Above/Renderer/Renderer2D.h>

#include <glm/glm.hpp>

#include <entt.hpp>

namespace Above
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());

		void OnUpdate(Timestep ts);
	private:
		entt::registry m_Registry; //entity IDs and component data

		friend class Entity;
	};
}