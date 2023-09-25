#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "SceneCamera.h"
#include <Above/Scene/ScriptableEntity.h>

#include <Above/Renderer/SubTexture2D.h>
#include <Above/Renderer/Texture.h>

namespace Above
{
	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag) : Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		glm::mat4 Transform = glm::mat4(1);

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation) : Translation(translation) {}

		glm::mat4 GetTransform() const
		{
			glm::mat4 transform = glm::mat4(1.0f);
			
			transform = glm::translate(transform, Translation);

			transform = glm::rotate(transform, Rotation.x, { 1,0,0 });
			transform = glm::rotate(transform, Rotation.y, { 0,1,0 });
			transform = glm::rotate(transform, Rotation.z, { 0,0,1 });

			transform = glm::scale(transform, Scale);

			return transform;
		}
	};

	struct RenderTargetComponent
	{
		uint32_t RenderTextureID;

		RenderTargetComponent() = default;
		RenderTargetComponent(const RenderTargetComponent&) = default;
		RenderTargetComponent(const uint32_t& renderTexID) : RenderTextureID(renderTexID) {}
	};

	struct SpriteRendererComponent
	{
		Ref<Texture2D> Texture;
		Ref<SubTexture2D> Subtexture;

		glm::vec4 Color = glm::vec4(1);

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color) : Color(color) {}
		SpriteRendererComponent(const Ref<Texture2D> texture) : Texture(texture) {}
		SpriteRendererComponent(const Ref<SubTexture2D> subtexture) : Subtexture(subtexture) {}
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = false;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity*(*InstantiateScript)() = nullptr;
		void (*DestroyScript)(NativeScriptComponent*) = nullptr;

		template<typename T>
		void Bind()
		{
			InstantiateScript =	[]() {  return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript =		[](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};
}
