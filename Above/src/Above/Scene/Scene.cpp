#include <abpch.h>
#include <Above/Scene/Scene.h>

#include <Above/Scene/Entity.h>

namespace Above
{
	Scene::Scene()
	{

	}

	Scene::~Scene()
	{
		 
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = Entity{ m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		
		return entity;
	}

	void Scene::OnUpdate(Timestep ts)
	{
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawQuad(transform, sprite.Color);
		}
	}
}