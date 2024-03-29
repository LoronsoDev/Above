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
		m_SelectionContext = {};
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

		if(ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight))
		{
			if(ImGui::MenuItem("Create empty entity"))
			{
				m_Context->CreateEntity("New entity");
			}
			ImGui::EndPopup();
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
		ImGuiIO& io = ImGui::GetIO();
		auto italicFont = io.Fonts->Fonts[2];

		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGui::PushFont(italicFont);
		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0 | ImGuiTreeNodeFlags_OpenOnArrow);
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if(ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}
		ImGui::PopFont();

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem(0, ImGuiPopupFlags_MouseButtonRight))
		{
			if (ImGui::MenuItem("Delete entity"))
			{
				entityDeleted = true;
			}
			ImGui::EndPopup();
		}

		if(opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((void*)89123, flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}
		ImGui::Separator();

		if(entityDeleted)
		{
			m_Context->DestroyEntity(entity);
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
		}
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[1];

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

			ImGui::PushFont(boldFont);
			if(ImGui::Button("X", buttonSize))
			{
				values.x = resetValue;
			}
			ImGui::PopFont();
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

			ImGui::PushFont(boldFont);
			if (ImGui::Button("Y", buttonSize))
			{
				values.y = resetValue;
			}
			ImGui::PopFont();

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

			ImGui::PushFont(boldFont);
			if (ImGui::Button("Z", buttonSize))
			{
				values.z = resetValue;
			}
			ImGui::PopFont();

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

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_AllowItemOverlap;

		ImGui::PushID(name.c_str());

		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4,4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			
			bool open = ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), treeNodeFlags, name.c_str());

			ImGui::PopStyleVar();

			ImGui::SameLine(contentRegionAvailable.x - lineHeight * .5f);
			if (ImGui::Button("...", ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
				{
					removeComponent = true;
				}

				ImGui::EndPopup();
			}

			if (open)
			{
				uiFunction(component);

				ImGui::TreePop();
			}

			ImGui::Separator();
			if (removeComponent)
			{
				entity.RemoveComponent<T>();
			}
		}

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

			if(ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		

		if (ImGui::Button("Add component"))
		{
			ImGui::OpenPopup("AddComp");
		}

		if (ImGui::BeginPopup("AddComp"))
		{
			if (ImGui::MenuItem("Camera"))
			{
				m_SelectionContext.AddComponent<CameraComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Sprite Renderer"))
			{
				m_SelectionContext.AddComponent<SpriteRendererComponent>();
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
		{
				DrawVec3Control("Translation", component.Translation);
				glm::vec3 rotation = glm::degrees(component.Rotation);
				DrawVec3Control("Rotation", rotation);
				component.Rotation = glm::radians(rotation);
				DrawVec3Control("Scale", component.Scale, 1.0f);
		});
		DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
		{
			auto& camera = component.Camera;

			ImGui::Checkbox("Primary", &component.Primary);

			const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
			const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
			if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
			{
				for (int i = 0; i < 2; ++i)
				{
					bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
					if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
					{
						currentProjectionTypeString = projectionTypeStrings[i];
						camera.SetProjectionType((SceneCamera::ProjectionType)i);
					}

					if (isSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}

				ImGui::EndCombo();
			}

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
			{
				float orthoSize = camera.GetOrthographicSize();

				if (ImGui::DragFloat("Size", &orthoSize, 0.05f))
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
				ImGui::Checkbox("Fixed aspect ratio", &component.FixedAspectRatio);
				if (component.FixedAspectRatio)
				{
					if (ImGui::DragFloat("Aspect ratio", &aspectRatio, 0.001f))
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
		});
		DrawComponent<RenderTargetComponent>("Render target", entity, [](auto& component)
		{
			ImGui::InputInt("Texture slot", (int*)& component.RenderTextureID);
		});
		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component)
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
		});
	}
}
