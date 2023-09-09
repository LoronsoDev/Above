#include <Above.h>

#include "Platform/OpenGL/OpenGLShader.h"
#include "imgui.h"

#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class ExampleLayer : public Above::Layer
{
public:
	ExampleLayer() 
		: Layer("Example"),
		m_Camera(-1.6f, 1.6f, -0.9f, 0.9f),
		m_CameraPosition(0.f)
	{
		m_VertexArray.reset(Above::VertexArray::Create());

		float vertices[3 * 7] =
		{
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		Above::Ref<Above::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Above::VertexBuffer::Create(vertices, sizeof(vertices)));
		//m_VertexBuffer->Bind();

		Above::BufferLayout layout =
		{
			{Above::ShaderDataType::Float3, "a_Position"},
			{Above::ShaderDataType::Float4, "a_Color"},
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Above::Ref<Above::IndexBuffer> indexBuffer;

		indexBuffer.reset(Above::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(Above::VertexArray::Create());

		float squareVertices[5 * 4] =
		{
			-.5f, -.5f, 0.0f, 0.0f, 0.0f,
			 .5f, -.5f, 0.0f, 1.0f, 0.0f,
			 .5f,  .5f, 0.0f, 1.0f, 1.0f,
			-.5f,  .5f, 0.0f, 0.0f, 1.0f
		};

		Above::Ref<Above::VertexBuffer> squareVB;
		squareVB.reset(Above::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout(
			{
			{Above::ShaderDataType::Float3, "a_Position"},
			{Above::ShaderDataType::Float2, "a_TexCoord"},
			});

		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Above::Ref<Above::IndexBuffer> squareIB;
		squareIB.reset(Above::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;	
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;	
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;		
		
			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";
		m_Shader.reset(Above::Shader::Create(vertexSrc, fragmentSrc));

		std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;	

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;	

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;		
		
			in vec3 v_Position;
			
			uniform vec3 u_Color;			

			void main()
			{
				color = vec4(u_Color, 1.0f);
			}
		)";

		m_FlatColorShader.reset(Above::Shader::Create(flatColorShaderVertexSrc, flatColorShaderFragmentSrc));

		m_TextureShader.reset(Above::Shader::Create("assets/shaders/Texture.glsl"));

		m_Texture = Above::Texture2D::Create("assets/textures/checkerboard.png");
		m_LogoTexture = Above::Texture2D::Create("assets/textures/logo.png");

		std::dynamic_pointer_cast<Above::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Above::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Above::Timestep ts) override
	{
		if (Above::Input::IsKeyPressed(AB_KEY_W))
		{
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		}
		if (Above::Input::IsKeyPressed(AB_KEY_S))
		{
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		}
		if (Above::Input::IsKeyPressed(AB_KEY_A))
		{
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		}
		if (Above::Input::IsKeyPressed(AB_KEY_D))
		{
			m_CameraPosition.x += m_CameraMoveSpeed * ts;
		}


		if (Above::Input::IsKeyPressed(AB_KEY_RIGHT))
		{
			m_CameraRotation -= m_CameraRotationSpeed * ts;
		}

		if (Above::Input::IsKeyPressed(AB_KEY_LEFT))
		{
			m_CameraRotation += m_CameraRotationSpeed * ts;
		}

		Above::RenderCommand::SetClearColor({ .1f, .1f, .1f, 1.0f });
		Above::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Above::Renderer::BeginScene(m_Camera);

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Above::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Above::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for(int y = 0; y < 20; ++y)
		{
			for (int x = 0; x < 20; ++x)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Above::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}

		m_Texture->Bind();
		Above::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_LogoTexture->Bind();
		Above::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// Triangle
		//Above::Renderer::Submit(m_Shader, m_VertexArray);

		Above::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Above::Event& event) override
	{
		//Above::EventDispatcher dispatcher(event);
	}

private:
	Above::Ref<Above::Shader> m_Shader;
	Above::Ref<Above::VertexArray> m_VertexArray;

	Above::Ref<Above::Shader> m_FlatColorShader, m_TextureShader;
	Above::Ref<Above::VertexArray> m_SquareVA;

	Above::Ref<Above::Texture2D> m_Texture, m_LogoTexture;

	Above::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraRotation = 0.0f;
	float m_CameraMoveSpeed = 2.f;
	float m_CameraRotationSpeed = 90.f;

	glm::vec3 m_SquareColor = glm::vec3(0.2f, 0.3f, 0.8f);
};

class Sandbox : public Above::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox() {}
};

Above::Application* Above::CreateApplication()
{
	AB_INFO("APPLICATION INITIALIZED");
	
	return new Sandbox;
}
