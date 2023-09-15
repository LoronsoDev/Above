#include <abpch.h>
#include <Above/Scene/Entity.h>

namespace Above
{
	Entity::Entity(entt::entity handle, Scene* scene)
		:
		m_EntityHandle(handle),
		m_Scene(scene)
	{

	}
}
