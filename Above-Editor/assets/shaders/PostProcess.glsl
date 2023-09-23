//Test comment

#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TexTiling;

out vec2 v_TexCoord;
out vec4 v_Color;
out float v_TexIndex;
out float v_TexTiling;

void main()
{
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
	v_TexTiling = a_TexTiling;

	gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
in float v_TexTiling;

uniform sampler2D u_RenderTexture;

void main()
{
	color = texture(u_RenderTexture, v_TexCoord);
}