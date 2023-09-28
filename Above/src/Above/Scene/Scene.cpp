#include <abpch.h>
#include <Above/Scene/Scene.h>

#include <Above/Scene/Components.h>
#include <Above/Renderer/Renderer2D.h>

#include <glm/glm.hpp>

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

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdate(Timestep ts)
	{
		//Update scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
				{
					//TODO: Move to OnScenePlay or similar
					if(!nsc.Instance)
					{
						nsc.Instance = nsc.InstantiateScript();
						nsc.Instance->m_Entity = Entity{ entity, this };
						nsc.Instance->OnCreate();
					}

					nsc.Instance->OnUpdate(ts);
				});
		}


		//Render 2D
		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		// Render sprites
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();
					break;
				}

			}
		}
		
		if (mainCamera)
		{
			Renderer2D::BeginScene(mainCamera->GetProjection(), cameraTransform);

			{
				auto group = m_Registry.group<>(entt::get<TransformComponent, SpriteRendererComponent>);
				for (auto entity : group)
				{
					auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

					if (sprite.Texture)
						Renderer2D::DrawQuad(transform.GetTransform(), sprite.Texture, 1, sprite.Color);
					else if (sprite.Subtexture)
						Renderer2D::DrawQuad(transform.GetTransform(), sprite.Subtexture, 1, sprite.Color);
					else
						Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
				}

			}

			Renderer2D::EndScene();
		}

		//Renderer2D::BeginScene(glm::mat4(1.0f), glm::mat4(1));
		////Post process, called after rendering everything.
		//{
		//	auto group = m_Registry.group<>(entt::get<TransformComponent, RenderTargetComponent>);
		//	for (auto entity : group)
		//	{
		//		auto [transform, renderTarget] = group.get<TransformComponent, RenderTargetComponent>(entity);

		//		Renderer2D::DrawRenderTarget(renderTarget.RenderTextureID);
		//	}
		//}
		//Renderer2D::EndScene();
	}


	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		//Resize our cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if(!cameraComponent.FixedAspectRatio)
			{
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}
	}


	template <typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
		
	}


	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		if(m_ViewportWidth > 0.0f && m_ViewportHeight > 0.0f)
		{
			component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
		}
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<RenderTargetComponent>(Entity entity, RenderTargetComponent& component)
	{
	}
}