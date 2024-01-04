#shader VERTEX
#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

//uniform mat4 u_MVP;

void main()
{
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0f);
	v_TexCoord = texCoord;
};

#shader FRAGMENT
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main()
{
	color = texture(u_Texture, v_TexCoord);
	//color = vec4(v_TexCoord.x, v_TexCoord.y, 0.0f, 1.0f); //DEBUG color checker
};