#shader VERTEX
#version 460 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Color;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

out vec4 v_Color;

void main()
{
	gl_Position = u_ViewProjection * vec4(a_Position.x, a_Position.y, a_Position.z, 1.0f);
	v_Color = a_Color;
};

#shader FRAGMENT
#version 460 core

layout(location = 0) out vec4 o_Color;

in vec4 v_Color;

void main()
{
	o_Color = v_Color;
};