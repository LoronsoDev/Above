#pragma once

#include <Above/Scene/Entity.h>

namespace Above
{
	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() = default;

		template <typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep timestep) {}

	private:
		Entity m_Entity;
		friend class Scene;
	};
}