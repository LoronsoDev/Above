#pragma once

#include <Above/Scene/Scene.h>

#include <entt.hpp>

namespace Above
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);

		template <typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		template <typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			AB_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template <typename T>
		T& GetComponent()
		{
			AB_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template <typename T>
		void RemoveComponent()
		{
			AB_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }
		operator entt::entity() const { return m_EntityHandle; }

		bool operator==(const Entity& other) const { return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }
		bool operator!=(const Entity& other) const { return !(*this == other); }

	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr; //should be weak_ptr but I want to rewrite the garbage collector so let's just use pointers for now
	};
}