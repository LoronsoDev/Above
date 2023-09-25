#include "abpch.h"
#include "GenericGuiLayer.h"
#include "Above/Core/Layer.h"

#include "imgui.h"
#include "Above/Core/Application.h"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

// TEMPORARY (windows dependant)
#include <GLFW/glfw3.h>

namespace Above
{
	GenericGuiLayer::GenericGuiLayer() : Layer("GenericGuiLayer")
	{
	}

	GenericGuiLayer::~GenericGuiLayer()
	{

	}

    void GenericGuiLayer::SetDarkThemeColors()
    {
        auto& colors = ImGui::GetStyle().Colors;

#define RGB888_TO_FLOAT(R,G,B) ImVec4{R/255.f, G/255.f, B/255.f, 1.0f}
#define rgb(R,G,B) RGB888_TO_FLOAT(R,G,B)
#define RGB(R,G,B) RGB888_TO_FLOAT(R,G,B)

        colors[ImGuiCol_WindowBg] = RGB(23, 26, 33);
        colors[ImGuiCol_HeaderHovered] = RGB(102, 192, 244);
        colors[ImGuiCol_HeaderActive] = RGB(27, 40, 56);

        colors[ImGuiCol_Button] = RGB(42, 71, 94);
        colors[ImGuiCol_ButtonHovered] = RGB(102, 192, 244);
        colors[ImGuiCol_ButtonActive] = RGB(27, 40, 56);

        colors[ImGuiCol_FrameBg] = RGB(27, 40, 56);
        colors[ImGuiCol_FrameBgHovered] = rgb(45, 66, 99);
        colors[ImGuiCol_FrameBgActive] = rgb(45, 66, 99);

        colors[ImGuiCol_Tab] = RGB(27, 40, 56);
        colors[ImGuiCol_TabActive] = RGB(102, 192, 244);
        colors[ImGuiCol_TabHovered] = RGB(199, 213, 224);

        colors[ImGuiCol_TabUnfocused] = RGB(27, 40, 56);
        colors[ImGuiCol_TabUnfocusedActive] = RGB(42, 71, 94);
        colors[ImGuiCol_TitleBg] = RGB(27, 40, 56);
        colors[ImGuiCol_TitleBgActive] = RGB(27, 40, 56);
        colors[ImGuiCol_TitleBgCollapsed] = RGB(27, 40, 56);


        //Steam classic
        /*colors[ImGuiCol_WindowBg] = RGB(63, 71, 56)
        colors[ImGuiCol_HeaderHovered] = RGB(76, 88, 68)
        colors[ImGuiCol_HeaderActive] = RGB(76, 88, 68)

        colors[ImGuiCol_Button] = RGB(76, 88, 68)
        colors[ImGuiCol_ButtonHovered] = RGB(150, 135, 50)
        colors[ImGuiCol_ButtonActive] = RGB(150, 135, 50)

        colors[ImGuiCol_FrameBg] = 	RGB(76, 88, 68)
        colors[ImGuiCol_FrameBgHovered] = rgb(45, 66, 99);
        colors[ImGuiCol_FrameBgActive] = rgb(45, 66, 99);

        colors[ImGuiCol_Tab] = rgb(236, 219, 186);
        colors[ImGuiCol_TabActive] = RGB(150, 135, 50)
        colors[ImGuiCol_TabHovered] = RGB(150, 135, 50)

        colors[ImGuiCol_TabUnfocused] = RGB(239, 246, 238)
        colors[ImGuiCol_TabUnfocusedActive] = RGB(63, 71, 56)
        colors[ImGuiCol_TitleBg] = RGB(63, 71, 56)
        colors[ImGuiCol_TitleBgActive] = RGB(63, 71, 56)
        colors[ImGuiCol_TitleBgCollapsed] = RGB(63, 71, 56)*/
    }


	void GenericGuiLayer::OnAttach()
	{
        AB_PROFILE_FUNCTION();

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;

        io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/WorkSans/WorkSans-Regular.ttf", 15.0f);
        io.Fonts->AddFontFromFileTTF("assets/fonts/WorkSans/WorkSans-Bold.ttf", 15.0f);
        io.Fonts->AddFontFromFileTTF("assets/fonts/WorkSans/WorkSans-MediumItalic.ttf", 15.0f);
        
        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        SetDarkThemeColors();

        Application& app = Application::Get();
        GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
	}

	void GenericGuiLayer::OnDetach()
	{
        AB_PROFILE_FUNCTION();

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
	}

    void GenericGuiLayer::OnEvent(Event& e)
    {
        if(m_BlockEvents)
        {
            ImGuiIO& io = ImGui::GetIO();
            e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
            e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureKeyboard;
        }
    }

    void GenericGuiLayer::Begin()
    {
        AB_PROFILE_FUNCTION();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void GenericGuiLayer::End()
    {
        AB_PROFILE_FUNCTION();

        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

}