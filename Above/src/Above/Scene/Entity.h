#pragma once

#include <Above/Scene/Scene.h>

namespace Above
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other);
		~Entity();

		template <typename T>
		void HasComponent()
		{
			m_Scene->m_Registry.has<T>(m_EntityHandle);
		}

		template <typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			AB_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template <typename T>
		T& GetComponent()
		{
			AB_CORE_ASSERT(!HasComponent<T>(), "Entity does not have component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template <typename T>
		void RemoveComponent()
		{
			AB_CORE_ASSERT(!HasComponent<T>(), "Entity does not have component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		operator bool() const { return exists; }

	private:
		bool exists = false;
		entt::entity m_EntityHandle;
		Scene* m_Scene = nullptr; //should be weak_ptr but I want to rewrite the garbage collector so let's just use pointers for now
	};
}