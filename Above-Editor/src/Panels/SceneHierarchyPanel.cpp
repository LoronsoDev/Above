#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

namespace Above
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		{
			m_Context->m_Registry.each([&](auto entityID)
			{
				Entity entity{ entityID, m_Context.get() };
				DrawEntityNode(entity);
			});
		}

		if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(0))
		{
			m_SelectionContext = {};
		}

		ImGui::End();

		ImGui::Begin("Inspector");
		{
			if(m_SelectionContext)
			{
				DrawComponents(m_SelectionContext);
			}
		}
		
		ImGui::End();
	}


	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0 | ImGuiTreeNodeFlags_OpenOnArrow);
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if(ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		if(opened)
		{
			ImGui::TreePop();
		}
		ImGui::Separator();
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.f)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		{
			ImGui::Text(label.c_str());
			ImGui::NextColumn();
		}

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0,0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.6f,0.1f,0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.7f,0.2f,0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f,0.3f,0.3f, 1.0f });

			if(ImGui::Button("X", buttonSize))
			{
				values.x = resetValue;
			}
			ImGui::SameLine();
			ImGui::DragFloat("##X", &values.x, 0.25f, 0.0f, 0.0f, "%.3f");
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PopStyleColor(3);
		}

		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f,0.6f,0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f,0.7f,0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.3f,0.8f,0.3f, 1.0f });

			if (ImGui::Button("Y", buttonSize))
			{
				values.y = resetValue;
			}
			ImGui::SameLine();
			ImGui::DragFloat("##Y", &values.y, 0.5f, 0.0f, 0.0f, "%.3f");
			ImGui::PopItemWidth();
			ImGui::SameLine();
			ImGui::PopStyleColor(3);
		}

		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.15f,0.1f,0.6f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f,0.2f,0.7f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.3f,0.3f,1.f, 0.8f });

			if (ImGui::Button("Z", buttonSize))
			{
				values.z = resetValue;
			}
			ImGui::SameLine();

			ImGui::DragFloat("##Z", &values.z, 0.05f, 0.0f, 0.0f, "%.3f");
			ImGui::PopItemWidth();
			ImGui::SameLine();
			ImGui::PopStyleColor(3);
		}

		ImGui::PopStyleVar();
		ImGui::Columns(1);

		ImGui::PopID();
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if(entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());

			if(ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
			ImGui::Separator();
		}

		if (entity.HasComponent<TransformComponent>())
		{
			if(ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& transform = entity.GetComponent<TransformComponent>();

				DrawVec3Control("Translation", transform.Translation);
				glm::vec3 rotation = glm::degrees(transform.Rotation);
				DrawVec3Control("Rotation", rotation);
				transform.Rotation = glm::radians(rotation);
				DrawVec3Control("Scale", transform.Scale, 1.0f);

				ImGui::TreePop();
			};
			ImGui::Separator();
		}

		if (entity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				auto& cc = entity.GetComponent<CameraComponent>();
				auto& camera = cc.Camera;

				ImGui::Checkbox("Primary", &cc.Primary);

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
				if(ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for(int i = 0; i < 2; ++i)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if(ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if(isSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}

					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = camera.GetOrthographicSize();

					if(ImGui::DragFloat("Size", &orthoSize, 0.05f))
					{
						camera.SetOrthographicSize(orthoSize);
					}

					float orthoNear = camera.GetOrthographicNearClip();

					if (ImGui::DragFloat("Near", &orthoNear, 0.05f))
					{
						camera.SetOrthographicNearClip(orthoNear);
					}

					float orthoFar = camera.GetOrthographicFarClip();

					if (ImGui::DragFloat("Far", &orthoFar, 0.05f))
					{
						camera.SetOrthographicFarClip(orthoFar);
					}

					static float originalAspectRatio = camera.GetAspectRatio();
					float aspectRatio = camera.GetAspectRatio();
					ImGui::Checkbox("Fixed aspect ratio", &cc.FixedAspectRatio);
					if(cc.FixedAspectRatio)
					{
						if(ImGui::DragFloat("Aspect ratio", &aspectRatio, 0.001f))
						{
							camera.SetAspectRatio(aspectRatio);
						}
					}
					else
					{
						camera.SetAspectRatio(originalAspectRatio);
					}
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float fov = glm::degrees(camera.GetPerspectiveVerticalFOV());

					if (ImGui::DragFloat("Horizontal FOV", &fov, 0.05f))
					{
						camera.SetPerspectiveVerticalFOV(glm::radians(fov));
					}

					float perspNear = camera.GetPerspectiveNearClip();

					if (ImGui::DragFloat("Near", &perspNear, 0.05f))
					{
						camera.SetPerspectiveNearClip(perspNear);
					}

					float perspFar = camera.GetPerspectiveFarClip();



					if (ImGui::DragFloat("Far", &perspFar, 0.05f))
					{
						camera.SetPerspectiveFarClip(perspFar);
					}

				}

				ImGui::TreePop();
			};
			ImGui::Separator();
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer"))
			{
				auto& sr = entity.GetComponent<SpriteRendererComponent>();

				ImGui::ColorEdit4("Color", glm::value_ptr(sr.Color));

				ImGui::TreePop();
			};
			ImGui::Separator();
		}
	}
}
